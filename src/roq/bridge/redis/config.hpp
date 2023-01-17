/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

namespace roq {
namespace bridge {
namespace redis {

struct Config final : public client::Config {
  Config() {}

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &) const override;
};

}  // namespace redis
}  // namespace bridge
}  // namespace roq
