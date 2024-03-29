/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/bridge/redis/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit",
    "exchange");

ABSL_FLAG(  //
    std::string,
    symbol,
    ".*",
    "symbol");

ABSL_FLAG(  //
    std::string,
    redis_address,
    "tcp://*:1234",
    "redis address");

ABSL_FLAG(  //
    uint16_t,
    redis_port,
    6379,
    "redis port");

ABSL_FLAG(  //
    uint32_t,
    mbp_depth,
    5,
    "mbp depth");

namespace roq {
namespace bridge {
namespace redis {
namespace flags {

std::string_view Flags::exchange() {
  static std::string const result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbol() {
  static std::string const result = absl::GetFlag(FLAGS_symbol);
  return result;
}

std::string_view Flags::redis_address() {
  static std::string const result = absl::GetFlag(FLAGS_redis_address);
  return result;
}

uint16_t Flags::redis_port() {
  static uint16_t const result = absl::GetFlag(FLAGS_redis_port);
  return result;
}

uint32_t Flags::mbp_depth() {
  static uint32_t const result = absl::GetFlag(FLAGS_mbp_depth);
  return result;
}

}  // namespace flags
}  // namespace redis
}  // namespace bridge
}  // namespace roq
