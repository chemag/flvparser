# cmake_minimum_required(VERSION <specify CMake version here>)
project(flvparser)

set(CMAKE_CXX_STANDARD 14)

add_executable(flvparser main.cpp FLVParser.cpp FLVHeader.cpp FLVTag.cpp FLVScriptTag.cpp FLVVideoTag.cpp FLVAudioTag.cpp FLVBaseTagData.cpp FLVDataTagFactory.cpp)

target_link_libraries(flvparser PRIVATE h264nal)
target_include_directories(flvparser PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib/h264nal"
                          )
include_directories(PUBLIC ../include/)
