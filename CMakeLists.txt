cmake_minimum_required(VERSION 3.10)
project(TradingStrategyOptimizer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/models.cpp
    src/indicators.cpp
    src/backtester.cpp
    src/optimizers.cpp
)

# Create executable
add_executable(optimizer ${SOURCES})

# Add threading library
find_package(Threads REQUIRED)
target_link_libraries(optimizer PRIVATE Threads::Threads)

# Set up output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create results directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/results)

# Installation
install(TARGETS optimizer DESTINATION bin)

# Add a command to copy example data files (optional)
# file(COPY ${CMAKE_SOURCE_DIR}/data/ DESTINATION ${CMAKE_BINARY_DIR}/data)

# Enable testing (optional)
# enable_testing()
# add_subdirectory(tests)

# Print a build summary
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")