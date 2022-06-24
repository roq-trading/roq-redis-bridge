/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/third_party/hiredis/context.hpp"

#include <hiredis/adapters/libevent.h>

#include <cassert>
#include <string>

#include "roq/exceptions.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace third_party {
namespace hiredis {

namespace {
using value_type = Context::value_type;

void deleter(value_type *ptr) {
  if (ptr)
    redisAsyncFree(ptr);
}

template <typename R>
R create(std::string const &addr, int port) {
  R result{redisAsyncConnect(addr.c_str(), port), deleter};
  if (!result)
    throw RuntimeError{"redisAsyncContext"sv};
  return result;
}

void connect_callback(const struct redisAsyncContext *context, int status) {
  if (status != REDIS_OK) {
    assert(context);
    log::error("REDIS ERROR: [{}] {}"sv, (*context).err, (*context).errstr);
  } else {
    // note! only on success
    assert((*context).data);
    auto &handler = *reinterpret_cast<Context::Handler *>((*context).data);
    handler(Context::Connected{});
  }
}

void disconnect_callback(const struct redisAsyncContext *context, int status) {
  assert(context);
  if (status != REDIS_OK && (*context).err != REDIS_ERR_EOF)
    log::error("REDIS ERROR: [{}] {}"sv, (*context).err, (*context).errstr);
  // note! always
  assert((*context).data);
  auto &handler = *reinterpret_cast<Context::Handler *>((*context).data);
  handler(Context::Disconnected{});
}
}  // namespace

Context::Context(Handler &handler, std::string_view const &addr, uint16_t port)
    : handle_(create<decltype(handle_)>(std::string{addr}, port)), handler_(handler) {
  (*handle_).data = &handler;
  auto result = redisAsyncSetConnectCallback(handle_.get(), connect_callback);
  if (result != REDIS_OK)
    throw RuntimeError{"redisAsyncSetConnectCallback [{}]"sv, result};
  result = redisAsyncSetDisconnectCallback(handle_.get(), disconnect_callback);
  if (result != REDIS_OK)
    throw RuntimeError{"redisAsyncSetDisconnectCallback [{}]"sv, result};
}

void Context::attach(struct event_base *base) {
  auto result = redisLibeventAttach(handle_.get(), base);
  if (result != REDIS_OK)
    throw RuntimeError{"redisLibeventAttach [{}]"sv, result};
}

void Context::send(std::string_view const &command) {
  std::string command_{command};  // XXX must be null-terminated !!!
  auto result = redisAsyncCommand(handle_.get(), nullptr, nullptr, command_.c_str());
  if (result != REDIS_OK)
    throw RuntimeError{"redisAsyncCommand [{}]"sv, result};
}

}  // namespace hiredis
}  // namespace third_party
}  // namespace roq
