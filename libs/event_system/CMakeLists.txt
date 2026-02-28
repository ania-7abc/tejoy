add_library(event_system SHARED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(event_system PRIVATE ${SOURCES})

target_include_directories(event_system PUBLIC include)

target_link_libraries(event_system PUBLIC Boost::system Boost::boost)
