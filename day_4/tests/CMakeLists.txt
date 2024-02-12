# Google Test


FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  all_test
  ScratchCard_unittest.cpp
  ScratchCardParser_unittest.cpp
  Day4Solver_unittest.cpp
  Utility_unittest.cpp
)


target_link_libraries(
  all_test
  GTest::gtest_main
  LIB_DAY4
)

include(GoogleTest)
gtest_discover_tests(all_test)