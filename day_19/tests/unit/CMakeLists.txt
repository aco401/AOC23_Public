# Always add tests from lowest to highest integration - if one fails with assert, others are not tested
add_executable(
  all_test_unit
  Utility_ut.cpp
  ThreadsafeQueue_ut.cpp
  ElapsedTimer_ut.cpp
  FileLineHandler_ut.cpp
  Workflow_ut.cpp
  WorkflowStringParser_ut.cpp
  WorkflowThread_ut.cpp
  PartCoordinator_ut.cpp
)

target_link_libraries(
  all_test_unit
  GTest::gtest_main
  LIB_DAY19
  LIB_LOGGING
  LIB_FILEHANDLING
  LIB_MEASURING
)

include(GoogleTest)
gtest_discover_tests(all_test_unit)