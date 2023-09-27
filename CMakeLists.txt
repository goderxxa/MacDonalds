cmake_minimum_required(VERSION 3.2)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp Game.cpp Game.h)
target_link_libraries(untitled sfml-graphics sfml-audio sfml-system sfml-window)

