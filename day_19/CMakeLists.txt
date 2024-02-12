cmake_minimum_required(VERSION 3.0.0)
project(aoc23_d17 VERSION 0.1.0 LANGUAGES CXX)

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

if(ENABLE_COVERAGE)
  # set compiler flags
  set(CMAKE_CXX_FLAGS "-O0 -coverage")

  # find required tools
  find_program(LCOV lcov REQUIRED)
  find_program(GENHTML genhtml REQUIRED)

  # add coverage target
  add_custom_target(coverage
    # gather data
    # --gcov-tool option is gcc compiler version dependent
    COMMAND ${LCOV} --directory . --capture --output-file coverage.info --gcov-tool /usr/bin/gcov-12
    # generate report
    COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

if(ENABLE_SANADDR) # TODO
  # set compiler flags
  #set(CMAKE_CXX_FLAGS "-O1 -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined")
  #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)

endif()

# Toogle Assert for production code
#add_compile_definitions(NDEBUG)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)