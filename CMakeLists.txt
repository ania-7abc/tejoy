cmake_minimum_required(VERSION 3.5...4.0)
project(tejoy
        VERSION 0.2.0
        LANGUAGES CXX
        DESCRIPTION "A cross-platform C++ library for building asynchronous messanger"
)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(tejoy_BUILD_EXAMPLES "Build tejoy examples" OFF)
option(tejoy_INSTALL "Enable installation of tejoy library" ON)

# Dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Boost 1.74.0 CONFIG REQUIRED COMPONENTS headers)
find_package(Sodium 1.0.22 REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)
find_package(event_bus 2.0.0 REQUIRED)

# Library
add_library(tejoy
        src/node.cpp
        src/modules/network.cpp
)

target_include_directories(tejoy PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(tejoy
        PRIVATE
        sodium

        PUBLIC
        nlohmann_json::nlohmann_json
        event_bus::event_bus
        Boost::headers
)

# Examples
if (tejoy_BUILD_EXAMPLES)
    message(STATUS "Building of examples is enabled")
    add_subdirectory(examples)
endif ()

# Install
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tejoyConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tejoy
        PATH_VARS INCLUDE_DIR
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

if (tejoy_INSTALL)
    install(TARGETS tejoy
            EXPORT tejoyTargets.cmake
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/tejoyConfigVersion.cmake
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindSodium.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tejoy
    )

    install(EXPORT tejoyTargets.cmake
            NAMESPACE tejoy::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tejoy
    )
else ()
    message(STATUS "Installation of tejoy is disabled")
endif ()
