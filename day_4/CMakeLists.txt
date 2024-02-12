cmake_minimum_required(VERSION 3.0.0)
project(aoc23_d4 VERSION 0.1.0 LANGUAGES CXX)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
include(FetchContent)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Verbose GCC Linking
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES ON)


# Toogle Assert for production code
#add_compile_definitions(NDEBUG)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)