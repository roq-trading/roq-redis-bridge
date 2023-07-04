/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/bridge/redis/application.hpp"

#include "roq/logging/flags/parser.hpp"
#include "roq/logging/flags/settings.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
auto const INFO = roq::Service::Info{
    .description = "Roq Redis Bridge"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_BUILD_VERSION,
    .build_number = ROQ_BUILD_NUMBER,
    .build_type = ROQ_BUILD_TYPE,
    .git_hash = ROQ_GIT_DESCRIBE_HASH,
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  roq::logging::flags::Parser parser{argc, argv, INFO.description, INFO.build_version};
  auto settings = roq::logging::flags::create_settings();
  return roq::bridge::redis::Application{parser, settings, INFO}.run();
}
