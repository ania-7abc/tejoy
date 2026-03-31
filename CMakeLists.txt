cmake_minimum_required(VERSION 3.20)
project(tejoy VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS ON)

# Find required external packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(nlohmann_json REQUIRED)

add_subdirectory(libs/tejoy)

include(FetchContent)

FetchContent_Declare(
    base64
    GIT_REPOSITORY https://github.com/ania-7abc/base64.git
    GIT_TAG v1.0.3
)
FetchContent_MakeAvailable(base64)

FetchContent_Declare(
    event_system
    GIT_REPOSITORY https://github.com/ania-7abc/event_system.git
    GIT_TAG v1.0.2
)
FetchContent_MakeAvailable(event_system)

FetchContent_Declare(
    secret_box
    GIT_REPOSITORY https://github.com/ania-7abc/secret_box.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(secret_box)

FetchContent_Declare(
    simple_io
    GIT_REPOSITORY https://github.com/ania-7abc/simple_io.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(simple_io)

FetchContent_Declare(
    storage
    GIT_REPOSITORY https://github.com/ania-7abc/storage.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(storage)

FetchContent_Declare(
    udp
    GIT_REPOSITORY https://github.com/ania-7abc/udp.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(udp)

target_link_libraries(tejoy PRIVATE
    nlohmann_json::nlohmann_json
    base64
    event_system
    secret_box
    simple_io
    storage
    udp
)

add_subdirectory(src)
