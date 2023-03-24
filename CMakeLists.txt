# CMakeLists files in this project can
# refer to the root source directory of the project as ${MAIN_SOURCE_DIR} and
# to the root binary directory of the project as ${MAIN_BINARY_DIR}.
cmake_minimum_required(VERSION 3.0)

# https://cmake.org/cmake/help/v3.0/command/project.html
project(flvparser VERSION 0.1)

# std::make_ptr requires C++14
set(CMAKE_CXX_STANDARD 14)

# set up clang-tidy
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-format-style='google'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

enable_testing()

# Recurse into library subdirectories.
add_subdirectory(lib/h264nal)

# Recurse into source code subdirectories.
add_subdirectory(src)
