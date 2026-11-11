cmake_minimum_required(VERSION 3.16)
project(modules_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

####### log

add_executable(log_formatter_test tests/log/formatter.cc)
target_include_directories(log_formatter_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

####### tests

add_test(NAME log_formatter_test COMMAND log_formatter_test)