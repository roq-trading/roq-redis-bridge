/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/bridge/redis/config.hpp"

#include "roq/bridge/redis/flags/flags.hpp"

namespace roq {
namespace bridge {
namespace redis {

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = flags::Flags::symbol(),
      .exchange = flags::Flags::exchange(),
  });
}

}  // namespace redis
}  // namespace bridge
}  // namespace roq
