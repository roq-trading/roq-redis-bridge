/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

namespace roq {
namespace adapter {
namespace redis {

class Config final : public client::Config {
 public:
  Config() {}

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace redis
}  // namespace adapter
}  // namespace roq
