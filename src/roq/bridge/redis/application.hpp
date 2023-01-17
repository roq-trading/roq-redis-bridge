/* Copyright (c) 2017-2023, Hans Erik Thrane */

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
  int main_helper(std::span<std::string_view> const &args);
  int main(int argc, char **argv) override;
};

}  // namespace redis
}  // namespace bridge
}  // namespace roq
