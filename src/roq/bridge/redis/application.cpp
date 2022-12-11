/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/bridge/redis/application.hpp"

#include <vector>

#include "roq/exceptions.hpp"

#include "roq/bridge/redis/config.hpp"
#include "roq/bridge/redis/controller.hpp"

using namespace std::literals;

namespace roq {
namespace bridge {
namespace redis {

// === IMPLEMENTATION ===

int Application::main_helper(std::span<std::string_view> const &args) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments should be a list of unix domain sockets
  auto connections = args.subspan(1);
  client::Bridge{config, connections}.dispatch<Controller>();
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  // wrap arguments (prefer to not work with raw pointers)
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace redis
}  // namespace bridge
}  // namespace roq
