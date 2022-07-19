#!/usr/bin/env bash

# debug?

if [ "$1" == "debug" ]; then
  KERNEL="$(uname -a)"
  case "$KERNEL" in
    Linux*)
      PREFIX="gdb --args"
      ;;
    Darwin*)
      PREFIX="lldb --"
      ;;
  esac
  shift 1
else
	PREFIX=
fi

# launch

$PREFIX ./roq-redis-bridge \
  --name trader \
  --exchange deribit \
  --symbol "^BTC-PERP.*$" \
  --redis_address "localhost" \
  --redis_port 6379 \
  ~/run/deribit.sock \
  $@
