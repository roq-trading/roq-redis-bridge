/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <event2/event.h>

#include <memory>

namespace roq {
namespace third_party {
namespace event {

struct Base final {
  using value_type = struct event_base;

  Base();

  operator value_type *() { return handle_.get(); }
  operator value_type const *() const { return handle_.get(); }

  void dispatch();

  void loop(int flags);

  void loopexit(const struct timeval *timeout = nullptr);

  void loopbreak();

 private:
  std::unique_ptr<value_type, void (*)(value_type *)> handle_;
};

}  // namespace event
}  // namespace third_party
}  // namespace roq
