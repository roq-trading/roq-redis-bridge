/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <hiredis/async.h>

#include <event2/event.h>

#include <memory>
#include <string_view>

namespace roq {
namespace third_party {
namespace hiredis {

struct Context final {
  using value_type = redisAsyncContext;

  struct Connected final {};
  struct Disconnected final {};
  struct Handler {
    virtual void operator()(Connected const &) = 0;
    virtual void operator()(Disconnected const &) = 0;
  };

  Context(Handler &, std::string_view const &addr, uint16_t port);

  operator value_type *() { return handle_.get(); }
  operator value_type const *() const { return handle_.get(); }

  void attach(struct event_base *);

  void send(std::string_view const &command);

 private:
  std::unique_ptr<value_type, void (*)(value_type *)> handle_;
  Handler &handler_;
};

}  // namespace hiredis
}  // namespace third_party
}  // namespace roq
