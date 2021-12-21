cmake_minimum_required(VERSION 3.10)

project(Signature)

find_package(Boost 1.74 COMPONENTS program_options REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-O3 -Wall)

add_executable(signature signature.cpp)
target_link_libraries(signature Boost::program_options)

add_executable(signature_test signature_test.cpp)
