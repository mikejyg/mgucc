cmake_minimum_required(VERSION 2.8)

project(mgucc CXX C)

add_compile_options(--std=c++11 -Wall)

include_directories(include)

file(GLOB_RECURSE srcs testSrc/*.cpp)
add_executable(mgucc ${srcs})
target_link_libraries(mgucc pthread)
