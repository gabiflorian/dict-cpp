cmake_minimum_required(VERSION 3.11)
project(dict-cpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module for dependency management
include(FetchContent)

# Library
add_library(dict-cpp INTERFACE)

target_include_directories(dict-cpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Examples
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_executable(example_basic examples/basic.cpp)
    target_link_libraries(example_basic dict-cpp)
    
    add_executable(example_parsing examples/parsing.cpp)
    target_link_libraries(example_parsing dict-cpp)
endif()

# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    
    add_executable(unit_tests tests/unit_tests.cpp)
    target_link_libraries(unit_tests dict-cpp)
    
    add_test(NAME unit_tests COMMAND unit_tests)
endif()

# Benchmarks
option(BUILD_BENCHMARKS "Build benchmarks" ON)
if(BUILD_BENCHMARKS)
    # Fetch nlohmann/json for benchmarks
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
        GIT_SHALLOW TRUE
    )
    
    # Make nlohmann/json available
    FetchContent_MakeAvailable(nlohmann_json)
    
    add_executable(benchmark benchmarks/benchmark.cpp)
    target_link_libraries(benchmark dict-cpp nlohmann_json::nlohmann_json)
endif()

# Installation
install(TARGETS dict-cpp
    EXPORT dict-cpp-targets
    INCLUDES DESTINATION include
)

install(DIRECTORY include/dict
    DESTINATION include
)

install(EXPORT dict-cpp-targets
    FILE dict-cpp-config.cmake
    NAMESPACE dict-cpp::
    DESTINATION lib/cmake/dict-cpp
)
