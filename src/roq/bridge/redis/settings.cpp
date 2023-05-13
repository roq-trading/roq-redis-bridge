/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/bridge/redis/settings.hpp"

#include "roq/logging.hpp"

#include "roq/bridge/redis/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace bridge {
namespace redis {

Settings::Settings(client::Type type)
    : client::Settings2{client::create_settings(type)}, exchange{flags::Flags::exchange()},
      symbol{flags::Flags::symbol()}, redis_address{flags::Flags::redis_address()},
      redis_port{flags::Flags::redis_port()}, mbp_depth{flags::Flags::mbp_depth()} {
  log::debug("settings={}"sv, *this);
}

}  // namespace redis
}  // namespace bridge
}  // namespace roq
