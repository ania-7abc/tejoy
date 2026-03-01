add_library(tejoy SHARED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
target_sources(tejoy PRIVATE ${SOURCES})

target_include_directories(tejoy PUBLIC include)

target_link_libraries(tejoy PUBLIC secret_box base64 storage event_system udp Boost::system Boost::boost)
