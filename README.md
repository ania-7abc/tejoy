# tejoy

[![Last Tag](https://img.shields.io/github/v/tag/ania-7abc/tejoy)](https://github.com/ania-7abc/tejoy/tags)
[![Stars](https://img.shields.io/github/stars/ania-7abc/tejoy)](https://github.com/ania-7abc/tejoy/stargazers)
[![Forks](https://img.shields.io/github/forks/ania-7abc/tejoy)](https://github.com/ania-7abc/tejoy/forks)
[![License](https://img.shields.io/github/license/ania-7abc/tejoy)](https://github.com/ania-7abc/tejoy/blob/main/LICENSE)
[![Docs](https://img.shields.io/badge/docs-tejoy-red)](https://ania-7abc.github.io/tejoy)

A cross-platform C++ library for building asynchronous messanger

## Dependencies
- [C++ 17](https://cppreference.com/cpp/17)
- [Boost.Asio 1.74.0](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html)
- [nlohmann/json](https://github.com/nlohmann/json) 3.11.3
- [event_bus](https://github.com/ania-7abc/event_bus) 1.0.3
- [libsodium](https://libsodium.org/) 1.0.22

## Installation
```bash
git clone https://github.com/ania-7abc/tejoy
cd tejoy
mkdir build && cd build
cmake ..
cmake --build . --config Release
cmake --install .
```

## Usage
When building your own project with CMake, you can find and link against tejoy:

```cmake
find_package(tejoy REQUIRED)

# Option A: link using the imported target (recommended)
target_link_libraries(your_target PRIVATE tejoy::tejoy)

# Option B: use the traditional variables
target_include_directories(your_target PRIVATE ${tejoy_INCLUDE_DIRS})
target_link_libraries(your_target PRIVATE ${tejoy_LIBRARIES})
```

For detailed documentation, examples, and API reference, please see the [`docs/`](./docs) folder  
You can build a beautiful static site with MkDocs (`mkdocs build`)

The latest documentation is also available [online](https://ania-7abc.github.io/tejoy)

## Contributing
Contributions are welcome! Please read our [CONTRIBUTING.md](./CONTRIBUTING.md) for guidelines on how to report issues, suggest features, and submit pull requests

## License
This project is licensed under the MIT License – see the [LICENSE](./LICENSE) file for details
