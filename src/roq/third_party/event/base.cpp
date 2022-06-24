/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/third_party/event/base.hpp"

#include <signal.h>

#include "roq/exceptions.hpp"

using namespace std::literals;

namespace roq {
namespace third_party {
namespace event {

namespace {
using value_type = Base::value_type;

void deleter(value_type *ptr) {
  if (ptr)
    event_base_free(ptr);
}

template <typename R>
R create() {
  R result{event_base_new(), deleter};
  if (!result)
    throw RuntimeError{"event_base_new"sv};
  return result;
}
}  // namespace

Base::Base() : handle_(create<decltype(handle_)>()) {
  ::signal(SIGPIPE, SIG_IGN);  // common enough to place here
}

void Base::dispatch() {
  if (event_base_dispatch(handle_.get()) < 0)
    throw RuntimeError{"event_base_dispatch"sv};
}

void Base::loop(int flags) {
  if (event_base_loop(handle_.get(), flags) < 0)
    throw RuntimeError{"event_base_loop"sv};
}

void Base::loopexit(const struct timeval *timeout) {
  if (event_base_loopexit(handle_.get(), timeout) < 0)
    throw RuntimeError{"event_base_loopexit"sv};
}

void Base::loopbreak() {
  if (event_base_loopbreak(handle_.get()) < 0)
    throw RuntimeError{"event_base_loopbreak"sv};
}

}  // namespace event
}  // namespace third_party
}  // namespace roq
