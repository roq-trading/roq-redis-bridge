/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/bridge/redis/application.hpp"

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
  return roq::bridge::redis::Application{argc, argv, INFO}.run();
}
