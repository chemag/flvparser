# cmake_minimum_required(VERSION <specify CMake version here>)
project(flvparser)

set(CMAKE_CXX_STANDARD 14)

add_executable(flvparser main.cpp FLVParser.cpp FLVParser.h FLVHeader.cpp FLVHeader.h FLVTag.cpp FLVTag.h FLVScriptTag.cpp FLVScriptTag.h FLVVideoTag.cpp FLVVideoTag.h FLVAudioTag.cpp FLVAudioTag.h FLVBaseTagData.cpp FLVBaseTagData.h FLVDataTagFactory.cpp FLVDataTagFactory.h)
