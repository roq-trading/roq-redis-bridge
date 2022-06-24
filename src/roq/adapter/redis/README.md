
```
./roq-redis-adapter \
  --name trader \
  --exchange deribit \
  --symbol "^BTC-PERP.*$" \
  --redis_address "localhost" \
  --redis_address 6379 \
  ~/run/deribit.sock \
  $@
```
