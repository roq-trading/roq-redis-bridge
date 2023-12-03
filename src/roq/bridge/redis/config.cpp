/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/bridge/redis/config.hpp"

namespace roq {
namespace bridge {
namespace redis {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = settings_.symbol,
      .exchange = settings_.exchange,
  });
}

}  // namespace redis
}  // namespace bridge
}  // namespace roq
