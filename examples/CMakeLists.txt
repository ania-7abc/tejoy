file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach(SOURCE_FILE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(tejoy_${EXAMPLE_NAME} ${SOURCE_FILE})
    target_link_libraries(tejoy_${EXAMPLE_NAME} PRIVATE tejoy)
    message(STATUS "Added example: ${EXAMPLE_NAME}")
endforeach()

option(tejoy_INSTALL_EXAMPLES "Install example source files" OFF)
