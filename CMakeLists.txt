cmake_minimum_required(VERSION 3.20)
project(tejoy VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required external packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSODIUM REQUIRED libsodium)
find_package(nlohmann_json REQUIRED)

# Add top-level include directory – used only by the executable
# We'll handle it in src/CMakeLists.txt to keep it modern, but a global include_directories is also possible.
# Here we do nothing globally; we'll add it directly to the executable target.

# Discover all library subdirectories under libs/ and add them if they have CMakeLists.txt
file(GLOB LIB_SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/libs ${CMAKE_CURRENT_SOURCE_DIR}/libs/*)
foreach(LIB_DIR ${LIB_SUBDIRS})
    set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/${LIB_DIR})
    if(IS_DIRECTORY ${LIB_PATH} AND EXISTS ${LIB_PATH}/CMakeLists.txt)
        add_subdirectory(libs/${LIB_DIR})
    endif()
endforeach()

# Add the executable subdirectory
add_subdirectory(src)
