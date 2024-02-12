# Google Test


FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Always add tests from lowest to highest integration - if one fails with assert, others are not tested
add_executable(
  all_test
  GraphAdjacentList_unittest.cpp
  VectorOfFile_unittest.cpp
  GraphFileHandler_unittest.cpp
  Algorithm_unittest.cpp
  Day17Solver_unittest.cpp
)


target_link_libraries(
  all_test
  GTest::gtest_main
  LIB_DAY17
)

include(GoogleTest)
gtest_discover_tests(all_test)