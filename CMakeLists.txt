cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp Game.cpp Game.h )
#add_executable(untitled test.cpp )
target_link_libraries(untitled sfml-graphics sfml-audio sfml-system sfml-window)

