/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/adapter/redis/config.hpp"

#include "roq/adapter/redis/flags/flags.hpp"

namespace roq {
namespace adapter {
namespace redis {

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = flags::Flags::symbol(),
      .exchange = flags::Flags::exchange(),
  });
}

}  // namespace redis
}  // namespace adapter
}  // namespace roq
