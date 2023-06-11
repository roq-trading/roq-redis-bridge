/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/client.hpp"

namespace roq {
namespace bridge {
namespace redis {

struct Settings final : public client::Settings2 {
  explicit Settings(client::Type);

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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::bridge::redis::Settings const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(redis_address="{}", )"
        R"(redis_port={}, )"
        R"(mbp_depth={}, )"
        R"(client={})"
        R"(}})"_cf,
        value.exchange,
        value.symbol,
        value.redis_address,
        value.redis_port,
        value.mbp_depth,
        static_cast<roq::client::Settings2 const &>(value));
  }
};