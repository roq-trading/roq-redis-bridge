# roq-redis-adapter

* Connects to one or more gateways.
* Converts messages to JSON and inserts these in a Redis memory database.


## Operating Systems

* Linux (x86-64, AArch64)
* macOS (x86-64, Arm64)


## Building

These are the steps required to build from source

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-x86_64.sh

bash Miniforge3-Linux-x86_64.sh -b -p opt/conda

source opt/conda/bin/activate

conda install -y \
    'gxx_linux-64>=12' \
    git \
    make \
    cmake \
    abseil-cpp \
    catch2 \
    libhiredis \
    libevent

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-client

git clone https://github.com/roq-trading/roq-redis-adapter

cd roq-redis-adapter

git submodule update --init --recursive

cmake .

make -j4
```

> This is a reference implementation.
> You are free to fork and modify the project if you need a custom format.


## Downloading

### Conda

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-x86_64.sh

bash Miniforge3-Linux-x86_64.sh -b -p opt/conda

source opt/conda/bin/activate

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-redis-adapter
```

### Docker

```bash
podman pull docker.io/roqtrading/roq-redis-adapter
```


## Using

### Conda

```bash
roq-redis-adapter \
    --name trader \
    --exchange deribit \
    --symbol "^BTC-PERP.*$" \
    --redis_address localhost \
    --redis_address 6379 \
    ~/run/deribit.sock
```

### Docker

```bash
podman exec -it docker.io/roqtrading/roq-redis-adapter \
    --name trader \
    --exchange deribit \
    --symbol "^BTC-PERP.*$" \
    --redis_address localhost \
    --redis_address 6379 \
    ~/run/deribit.sock
```


## Dependencies

* [Abseil-C++](https://github.com/abseil/abseil-cpp) (Apache 2.0 License)
* [Catch2](https://github.com/catchorg/Catch2) (BSL 1.0 License)
* [fmt](https://github.com/fmtlib/fmt) (MIT License)
* [hiredis](https://github.com/redis/hiredis) (BSD 3-Clause License)
* [libevent](https://github.com/libevent/libevent) (BSD 3-Clause License)
* [magic\_enum](https://github.com/Neargye/magic_enum) (MIT License)
* [range-v3](https://github.com/ericniebler/range-v3) (BSL 1.0 License)
* [span-lite](https://github.com/martinmoene/span-lite) (BSL 1.0 License)
* [roq-api](https://github.com/roq-trading/roq-api) (MIT License)
* [roq-logging](https://github.com/roq-trading/roq-api) (MIT License)
* roq-client (Commerical License, free to use)


## License

The project is released under the terms of the BSD 3-Clause license.


## Links

* [Roq GmbH](https://roq-trading.com/)
* [Documentation](https://roq-trading.com/docs/)
* [Contact us](mailto:info@roq-trading.com)
