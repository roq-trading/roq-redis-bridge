/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace bridge {
namespace redis {

struct Settings final : public client::flags::Settings {
  explicit Settings(args::Parser const &);

  std::string_view exchange;
  std::string_view symbol;
  std::string_view redis_address;
  uint16_t redis_port = {};
  uint32_t mbp_depth = {};
};

}  // namespace redis
}  // namespace bridge
}  // namespace roq

template <>
struct fmt::formatter<roq::bridge::redis::Settings> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::bridge::redis::Settings const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(redis_address="{}", )"
        R"(redis_port={}, )"
        R"(mbp_depth={}, )"
        R"(client={})"
        R"(}})"sv,
        value.exchange,
        value.symbol,
        value.redis_address,
        value.redis_port,
        value.mbp_depth,
        static_cast<roq::client::Settings2 const &>(value));
  }
};
