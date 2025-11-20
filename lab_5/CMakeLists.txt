cmake_minimum_required(VERSION 3.10)
project(MAI-OOP-LABS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)

add_library(lab5_lib STATIC
    src/my_memory_resource.cpp
)

target_include_directories(lab5_lib PUBLIC include)

add_executable(lab5_exe main.cpp)
target_link_libraries(lab5_exe lab5_lib)

enable_testing()
add_executable(tests test/test05.cpp)
target_link_libraries(tests lab5_lib gtest_main)

add_test(NAME Lab05-Tests COMMAND tests)
