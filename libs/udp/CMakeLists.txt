add_library(udp SHARED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(udp PRIVATE ${SOURCES})

target_include_directories(udp PUBLIC include)

target_link_libraries(udp PUBLIC Boost::system Boost::boost)
