add_library(base64 SHARED)

# Collect source files – new .cpp files will be picked up automatically
file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(base64 PRIVATE ${SOURCES})

target_include_directories(base64 PUBLIC include)

# Link libsodium
target_link_libraries(base64 PUBLIC ${LIBSODIUM_LIBRARIES})
target_include_directories(base64 SYSTEM PRIVATE ${LIBSODIUM_INCLUDE_DIRS})
