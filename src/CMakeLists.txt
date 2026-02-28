add_executable(tejoy_demo main.cpp)

# Link all libraries
target_link_libraries(tejoy_demo PRIVATE
    tejoy
)

# Add top-level include directory for config.hpp
target_include_directories(tejoy_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Optional: set output directory for the executable
set_target_properties(tejoy_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
