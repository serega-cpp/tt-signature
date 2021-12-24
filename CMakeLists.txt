cmake_minimum_required(VERSION 3.10)

project(Signature)

find_package(Threads REQUIRED)
find_package(Boost 1.74 COMPONENTS program_options REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if (MSVC)
    add_compile_options("$<$<CONFIG:RELEASE>:/W4;-WX;-Ox>")
    add_definitions(-DBOOST_ALL_NO_LIB)
    add_definitions(-DBOOST_ALL_DYN_LINK)
else()
    add_compile_options("$<$<CONFIG:RELEASE>:-Wall;-Wextra;-O3>")
endif()

include_directories(${Boost_INCLUDE_DIR})

add_executable(signature signature.cpp)
target_link_libraries(signature Threads::Threads Boost::program_options)

add_executable(signature_test signature_test.cpp)
target_link_libraries(signature_test Threads::Threads)
