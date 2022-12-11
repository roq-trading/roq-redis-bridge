/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace bridge {
namespace redis {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static std::string_view redis_address();
  static uint16_t redis_port();
  static uint32_t mbp_depth();
};

}  // namespace flags
}  // namespace redis
}  // namespace bridge
}  // namespace roq
