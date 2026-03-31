# tejoy

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

A C++17 P2P networking library with modular architecture.

## Description

tejoy provides a foundation for building peer-to-peer applications with features like node discovery, encrypted messaging, and event-driven architecture.

## Requirements

- C++17 compiler
- CMake 3.14+

### System Dependencies

| Library | Purpose |
|---------|---------|
| [Boost](https://www.boost.org/) | Asio for networking |
| [libsodium](https://doc.libsodium.org/) | Encryption |
| [nlohmann/json](https://json.nlohmann.me/) | JSON handling |

### FetchContent Dependencies

These are fetched automatically by CMake:

| Library | Repository |
|---------|------------|
| base64 | [ania-7abc/base64](https://github.com/ania-7abc/base64) |
| event_system | [ania-7abc/event_system](https://github.com/ania-7abc/event_system) |
| secret_box | [ania-7abc/secret_box](https://github.com/ania-7abc/secret_box) |
| storage | [ania-7abc/storage](https://github.com/ania-7abc/storage) |
| udp | [ania-7abc/udp](https://github.com/ania-7abc/udp) |

## Installation

### Using CMake FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    tejoy
    GIT_REPOSITORY https://github.com/ania-7abc/tejoy.git
    GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(tejoy)

target_link_libraries(your_target PRIVATE tejoy)
```

### Manual Build

```bash
# Install system dependencies (Ubuntu)
sudo apt-get install libboost-all-dev libsodium-dev nlohmann-json3-dev

# Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Install
sudo cmake --install build
```

## Documentation

Detailed API documentation and usage examples are available in the [Wiki](https://github.com/ania-7abc/tejoy/wiki).

## Quick Example

```cpp
#include <tejoy/node.hpp>

int main() {
    tejoy::Node node;
    node.start();
    return 0;
}
```

## License

MIT License -- see [LICENSE](LICENSE) for details.
