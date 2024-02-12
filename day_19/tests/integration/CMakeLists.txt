


add_executable(
  all_test_integration
  ThreadsafeQueue_it.cpp
  PartCoordinator_it.cpp
  ThreadCoordinator_it.cpp
  Day19Solver_it.cpp
)

target_link_libraries(
  all_test_integration
  GTest::gtest_main
  LIB_DAY19
  LIB_LOGGING
  LIB_FILEHANDLING
  LIB_MEASURING
)

include(GoogleTest)
gtest_discover_tests(all_test_integration)