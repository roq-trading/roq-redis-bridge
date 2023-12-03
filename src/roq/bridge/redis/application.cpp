/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/bridge/redis/application.hpp"

#include <vector>

#include "roq/exceptions.hpp"

#include "roq/bridge/redis/config.hpp"
#include "roq/bridge/redis/controller.hpp"
#include "roq/bridge/redis/settings.hpp"

using namespace std::literals;

namespace roq {
namespace bridge {
namespace redis {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  Settings settings{args};
  Config config{settings};
  client::Bridge{settings, config, params}.dispatch<Controller>(settings);
  return EXIT_SUCCESS;
}

}  // namespace redis
}  // namespace bridge
}  // namespace roq
