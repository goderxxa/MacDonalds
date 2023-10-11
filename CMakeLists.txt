cmake_minimum_required(VERSION 3.23)
project(MacDonalds)

set(CMAKE_CXX_STANDARD 20)

add_executable(MacDonalds main.cpp Game.cpp Game.h )
#add_executable(MacDonalds test.cpp )
target_link_libraries(MacDonalds sfml-graphics sfml-audio sfml-system sfml-window)

