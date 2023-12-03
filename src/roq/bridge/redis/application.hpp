/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <span>

#include "roq/service.hpp"

namespace roq {
namespace bridge {
namespace redis {

// application

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace redis
}  // namespace bridge
}  // namespace roq
