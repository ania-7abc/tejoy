# Installation

This guide provides instructions for building and installing the tejoy library

## Dependencies

- [C++17](https://cppreference.com/cpp/17) – a compiler with full C++17 support is required
- [Boost.Asio 1.74.0](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html) – for asynchronous networking and
  timers
- [nlohmann/json 3.11.3](https://github.com/nlohmann/json) – for JSON serialization and parsing
- [event_bus 1.0.3](https://github.com/ania-7abc/event_bus) – internal event dispatch mechanism
- [libsodium 1.0.22](https://libsodium.org/) – for cryptographic operations and secure messaging

## Getting Sources

Clone the repository from GitHub:

```
git clone https://github.com/ania-7abc/tejoy.git
cd tejoy
```

Alternatively, you can download the source archive from the releases page

## Installation

Build and install using CMake. Run the following commands from the project root:

```
mkdir build && cd build
cmake ..
cmake --build . --config Release
cmake --install . --prefix /usr/local
```

Replace `/usr/local` with your preferred installation directory if needed

## Advanced

Several CMake options are available to customize the build:

- `BUILD_SHARED_LIBS` – set to `ON` to build shared libraries instead of static ones
- `tejoy_BUILD_EXAMPLES` – set to `ON` to compile the example programs
- `tejoy_INSTALL` – set to `OFF` to skip installation rules

You can pass these options during the initial `cmake` invocation, for example:

```
cmake -DBUILD_SHARED_LIBS=ON -Dtejoy_BUILD_EXAMPLES=ON ..
```
