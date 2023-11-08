CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

# Project config
project(
    graph-libraries-comparison
    VERSION 0.0.0
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/lucasguesserts/graph-libraries-comparison"
)

# Prevent building in the source directory
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

# Build options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Dependencies
## Cmake Package Manager
include(cmake/get_cpm.cmake)
## CPM dependencies
CPMAddPackage(
    NAME Catch2
    VERSION 3.4.0
    GITHUB_REPOSITORY catchorg/Catch2
    OPTIONS
        "CMAKE_CXX_FLAGS -fPIC"
)
## Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.78 REQUIRED COMPONENTS graph)
## Lemon
set(LEMON_DIR "/opt/lemon")
find_package(LEMON REQUIRED)

# Tests
include (CTest)

# Catch test
add_executable(Catch.test src/Catch.test.cpp)
target_link_libraries(Catch.test PRIVATE Catch2::Catch2WithMain)
target_compile_options(Catch.test PRIVATE -ggdb3 -Wall -Wextra -pedantic-errors -fmax-errors=1 -Wno-error=ignored-qualifiers)
target_compile_features(Catch.test PRIVATE cxx_std_17)
add_test(NAME Catch.test COMMAND Catch.test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Boost Graph Library test
add_executable(BoostGraphLibrary.test src/BoostGraphLibrary.test.cpp)
target_link_libraries(BoostGraphLibrary.test PRIVATE Catch2::Catch2WithMain Boost::graph)
target_compile_options(BoostGraphLibrary.test PRIVATE -ggdb3 -Wall -Wextra -pedantic-errors -fmax-errors=1 -Wno-error=ignored-qualifiers)
target_compile_features(BoostGraphLibrary.test PRIVATE cxx_std_17)
add_test(NAME BoostGraphLibrary.test COMMAND BoostGraphLibrary.test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Boost Graph Library test
add_executable(Lemon.test src/Lemon.test.cpp)
target_include_directories(Lemon.test PRIVATE ${LEMON_INCLUDE_DIRS})
target_link_libraries(Lemon.test PRIVATE Catch2::Catch2WithMain ${LEMON_LIBRARIES})
target_compile_options(Lemon.test PRIVATE -ggdb3 -Wall -Wextra -pedantic-errors -fmax-errors=1 -Wno-error=ignored-qualifiers)
target_compile_features(Lemon.test PRIVATE cxx_std_17)
add_test(NAME Lemon.test COMMAND Lemon.test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
