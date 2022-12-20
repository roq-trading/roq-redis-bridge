/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/bridge/redis/controller.hpp"

#include <memory>
#include <utility>

#include "roq/logging.hpp"

#include "roq/utils/get.hpp"

#include "roq/json/gateway_status.hpp"
#include "roq/json/market_by_price_update.hpp"
#include "roq/json/market_status.hpp"
#include "roq/json/reference_data.hpp"
#include "roq/json/statistics.hpp"
#include "roq/json/statistics_update.hpp"
#include "roq/json/top_of_book.hpp"
#include "roq/json/trade_summary.hpp"

#include "roq/io/engine/libevent/context_factory.hpp"

#include "roq/bridge/redis/key.hpp"

#include "roq/bridge/redis/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace bridge {
namespace redis {

// some notes about hiredis:
// - the context will only try to establish the connection *after* we send a message
// -- this is the reason for sending a heartbeat
// - the context doesn't appear to be re-usable after a disconnect
// -- this is why we have to recreate the context following a disconnect
// - the interactions between hiredis and libevent are complex
// -- this is why we use the zombie flag to delete the context outside the event loop

// === CONSTANTS ===

namespace {
auto const HEARTBEAT_FREQUENCY = 1s;
}

// === HELPERS ===

namespace {
auto create_context(auto &handler, auto &libevent) {
  auto result = std::make_unique<third_party::hiredis::Context>(
      handler, flags::Flags::redis_address(), flags::Flags::redis_port());
  (*result).attach(libevent);
  return result;
}
}  // namespace

// === IMPLEMENTATION ===

Controller::Controller(client::Dispatcher &dispatcher)
    : dispatcher_{dispatcher}, libevent_{io::engine::libevent::ContextFactory::create()},
      bids_(flags::Flags::mbp_depth()), asks_(flags::Flags::mbp_depth()) {
}

void Controller::operator()(Event<Start> const &) {
}

void Controller::operator()(Event<Stop> const &) {
  (*libevent_).stop();
}

// note!
// the timer event handler will be called in a round-robin loop when the
// application is running in bridge mode
void Controller::operator()(Event<Timer> const &event) {
  auto &[message_info, timer] = event;
  // heartbeat
  auto now = timer.now;
  if (next_heartbeat_ < now) {
    next_heartbeat_ = now + HEARTBEAT_FREQUENCY;
    send_ping(now);
  }
  // event-loop
  (*libevent_).drain();
  // disconnect?
  if (zombie_) {
    zombie_ = false;
    context_.reset();
  }
}

void Controller::operator()(Event<GatewayStatus> const &event) {
  if (ready())
    send("SET {} {}"sv, Key{event}, json::GatewayStatus{event});
}

void Controller::operator()(Event<MarketStatus> const &event) {
  if (ready())
    send("SET {} {}"sv, Key{event}, json::MarketStatus{event});
}

void Controller::operator()(Event<ReferenceData> const &event) {
  auto &reference_data = event.value;
  auto &context = update_json_context(
      reference_data.exchange, reference_data.symbol, reference_data.tick_size, reference_data.min_trade_vol);
  if (ready())
    send("SET {} {}"sv, Key{event}, json::ReferenceData{context, event});
}

void Controller::operator()(Event<TopOfBook> const &event) {
  if (ready()) {
    auto &context = get_json_context(event.value.exchange, event.value.symbol);
    send("SET {} {}"sv, Key{event}, json::TopOfBook{context, event});
  }
}

void Controller::operator()(Event<TradeSummary> const &event) {
  if (ready()) {
    auto &context = get_json_context(event.value.exchange, event.value.symbol);
    send("SET {} {}"sv, Key{event}, json::TradeSummary{context, event});
  }
}

void Controller::operator()(Event<StatisticsUpdate> const &event) {
  if (ready()) {
    get_market(event, [&](auto &market) {
      if (market(event)) {
        auto &context = get_json_context(event.value.exchange, event.value.symbol);
        send("SET {} {}"sv, Key{event}, json::StatisticsUpdate{context, event, market.statistics});
      }
    });
  }
}

void Controller::operator()(Event<MarketByPriceUpdate> const &event) {
  if (ready()) {
    get_market(event, [&](auto &market) {
      if (market(event)) {
        auto &context = get_json_context(event.value.exchange, event.value.symbol);
        auto [bids, asks] = (*market.market_by_price).extract(bids_, asks_, true);
        send("SET {} {}"sv, Key{event}, json::MarketByPriceUpdate{context, event, bids, asks});
      }
    });
  }
}

void Controller::operator()(third_party::hiredis::Context::Connected const &) {
  log::info(R"(Connected (host="{}", port={}))"sv, flags::Flags::redis_address(), flags::Flags::redis_port());
}

void Controller::operator()(third_party::hiredis::Context::Disconnected const &) {
  log::warn(R"(Disconnected (host="{}", port={}))"sv, flags::Flags::redis_address(), flags::Flags::redis_port());
  zombie_ = true;
}

void Controller::send_ping(std::chrono::nanoseconds now) {
  send("PING {}"sv, now);
}

template <typename... Args>
void Controller::send(fmt::format_string<Args...> const &fmt, Args &&...args) {
  if (zombie_)  // wait for timer
    return;
  buffer_.clear();
  fmt::format_to(std::back_inserter(buffer_), fmt, std::forward<Args>(args)...);
  std::string_view message{std::data(buffer_), std::size(buffer_)};
  log::info<3>("{}"sv, message);
  try {
    if (!context_)
      context_ = create_context(*this, *libevent_);
    (*context_).send(message);
  } catch (RuntimeError &e) {
    log::error("ERROR: {}"sv, e.what());
    zombie_ = true;
  }
}

template <typename T, typename Callback>
void Controller::get_market(Event<T> const &event, Callback callback) {
  auto &manager = get_manager(event.message_info);
  auto [market, _] = manager.get_market_or_create(event.value.exchange, event.value.symbol);
  callback(market);
}

cache::Manager &Controller::get_manager(MessageInfo const &message_info) {
  auto iter = cache_.find(message_info.source);
  if (iter == std::end(cache_))
    iter = cache_.emplace(message_info.source, client::MarketByPriceFactory::create).first;
  return (*iter).second;
}

json::Context const &Controller::get_json_context(std::string_view const &exchange, std::string_view const &symbol) {
  return json_context_[exchange][symbol];
}

json::Context const &Controller::update_json_context(
    std::string_view const &exchange, std::string_view const &symbol, double tick_size, double min_trade_vol) {
  auto &result = json_context_[exchange][symbol];
  result.price_decimals = client::Number::increment_to_decimals(tick_size);
  result.quantity_decimals = client::Number::increment_to_decimals(min_trade_vol);
  return result;
}

}  // namespace redis
}  // namespace bridge
}  // namespace roq
