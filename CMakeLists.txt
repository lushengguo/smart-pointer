cmake_minimum_required(VERSION 3.5)
project(smart_pointer) 

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unique_ptr_test unique_ptr_test.cc)
target_link_libraries(unique_ptr_test gtest_main)
add_test(NAME unique_ptr_test COMMAND unique_ptr_test)