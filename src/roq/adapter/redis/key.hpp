/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/api.hpp"

#include "roq/utils/get.hpp"

namespace roq {
namespace adapter {
namespace redis {

// key = <event_type>|<symbol>|<exchange>|<source>
struct Key final {
  template <typename T>
  constexpr Key(roq::Event<T> const &event)
      : type_(roq::get_name<T>()), source_(event.message_info.source_name),
        exchange_(roq::utils::get_exchange(event.value)), symbol_(roq::utils::get_symbol(event.value)) {}

  template <typename Context>
  constexpr auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}|{}|{}|{}"sv, type_, symbol_, exchange_, source_);
  }

 private:
  const std::string_view type_;
  const std::string_view source_;
  const std::string_view exchange_;
  const std::string_view symbol_;
};

}  // namespace redis
}  // namespace adapter
}  // namespace roq

template <>
struct fmt::formatter<roq::adapter::redis::Key> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::adapter::redis::Key const &value, Context &context) {
    using namespace std::literals;
    return value.format_to(context);
  }
};
