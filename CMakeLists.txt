cmake_minimum_required(VERSION 3.14)
project(tejoy VERSION 1.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_DOCS "Build Doxygen documentation" OFF)

add_library(tejoy)

target_sources(tejoy PRIVATE
    src/detail/modules/ack_module.cpp
    src/detail/modules/discovery_module.cpp
    src/detail/modules/module_manager.cpp
    src/detail/modules/network_module.cpp
    src/detail/modules/update_manager_module.cpp
)

target_sources(tejoy PRIVATE
    src/node.cpp
)

set_target_properties(tejoy PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
target_include_directories(tejoy PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(FetchContent)

FetchContent_Declare(
    base64
    GIT_REPOSITORY https://github.com/ania-7abc/base64
    GIT_TAG v1.0.3
)

FetchContent_Declare(
    event_system
    GIT_REPOSITORY https://github.com/ania-7abc/event_system
    GIT_TAG v1.0.2
)

FetchContent_Declare(
    secret_box
    GIT_REPOSITORY https://github.com/ania-7abc/secret_box
    GIT_TAG v1.0.0
)

FetchContent_Declare(
    storage
    GIT_REPOSITORY https://github.com/ania-7abc/storage
    GIT_TAG v1.0.0
)

FetchContent_Declare(
    udp
    GIT_REPOSITORY https://github.com/ania-7abc/udp
    GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(base64 event_system secret_box storage udp)

find_package(nlohmann_json REQUIRED)
target_link_libraries(tejoy PRIVATE
    base64
    event_system
    secret_box
    storage
    udp
    nlohmann_json::nlohmann_json
)

if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        doxygen_add_docs(doxygen-docs
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
        )
    else()
        message(SEND_ERROR "Doxygen not found")
    endif() 
endif()

install(TARGETS tejoy EXPORT tejoyTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
install(EXPORT tejoyTargets
    FILE tejoyTargets.cmake
    NAMESPACE ania-7abc::
    DESTINATION lib/cmake/tejoy
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tejoyConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfig.cmake
    INSTALL_DESTINATION lib/cmake/tejoy
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfigVersion.cmake
    DESTINATION lib/cmake/tejoy
)
