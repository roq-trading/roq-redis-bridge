/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/api.hpp"

#include "roq/utils/get.hpp"

namespace roq {
namespace bridge {
namespace redis {

// key = <event_type>|<symbol>|<exchange>|<source>
struct Key final {
  template <typename T>
  constexpr Key(roq::Event<T> const &event)
      : type_{roq::get_name<T>()}, source_{event.message_info.source_name},
        exchange_{roq::utils::get_exchange(event.value)}, symbol_{roq::utils::get_symbol(event.value)} {}

  constexpr auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}|{}|{}|{}"sv, type_, symbol_, exchange_, source_);
  }

 private:
  std::string_view const type_;
  std::string_view const source_;
  std::string_view const exchange_;
  std::string_view const symbol_;
};

}  // namespace redis
}  // namespace bridge
}  // namespace roq

template <>
struct fmt::formatter<roq::bridge::redis::Key> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::bridge::redis::Key const &value, format_context &context) const {
    using namespace std::literals;
    return value.format_to(context);
  }
};
