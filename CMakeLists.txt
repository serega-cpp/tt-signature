cmake_minimum_required(VERSION 3.10)

project(Signature)

find_package(Threads REQUIRED)
find_package(Boost 1.74 COMPONENTS program_options REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_compile_options(-O3 -Wall)
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(signature signature.cpp)
target_link_libraries(signature Boost::program_options Threads::Threads)

add_executable(signature_test signature_test.cpp)
target_link_libraries(signature_test Threads::Threads)

