cmake_minimum_required(VERSION 3.8)
project(sborka)

set(CMAKE_CXX_STANDARD 11)

find_library(CURL REQUIRED)

set(SOURCE_FILES
        stringParser/comfyParse.h
        stringParser/curlParse.h
        stringParser/stringParse.h
        main.cpp stringParser/Serializer.h)

add_executable(sborka ${SOURCE_FILES})
target_link_libraries(sborka curl)