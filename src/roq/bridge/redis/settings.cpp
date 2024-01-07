/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/bridge/redis/settings.hpp"

#include "roq/logging.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/bridge/redis/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace bridge {
namespace redis {

Settings::Settings(args::Parser const &args)
    : client::flags::Settings{args}, exchange{flags::Flags::exchange()}, symbol{flags::Flags::symbol()},
      redis_address{flags::Flags::redis_address()}, redis_port{flags::Flags::redis_port()},
      mbp_depth{flags::Flags::mbp_depth()} {
  // log::info("settings={}"sv, *this);
}

}  // namespace redis
}  // namespace bridge
}  // namespace roq
