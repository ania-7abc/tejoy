add_library(tejoy SHARED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(tejoy PRIVATE ${SOURCES})

target_include_directories(tejoy PUBLIC include)

target_link_libraries(tejoy PUBLIC storage event_system udp Boost::system Boost::boost)
