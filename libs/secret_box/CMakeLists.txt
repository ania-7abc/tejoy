add_library(secret_box SHARED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(secret_box PRIVATE ${SOURCES})

target_include_directories(secret_box PUBLIC include)

target_link_libraries(secret_box PUBLIC ${LIBSODIUM_LIBRARIES})
target_include_directories(secret_box SYSTEM PRIVATE ${LIBSODIUM_INCLUDE_DIRS})
