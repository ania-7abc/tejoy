add_library(storage SHARED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(storage PRIVATE ${SOURCES})

target_include_directories(storage PUBLIC include)

target_link_libraries(storage PUBLIC simple_io nlohmann_json::nlohmann_json)
