# CMakeList.txt : CMake project for HanoiTowers, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("HanoiTowers")

set(CMAKE_BUILD_TYPE "Release")

# Add source to this project's executable.
add_executable (HanoiTowers "HanoiTowers.cpp" )

# TODO: Add tests and install targets if needed.
