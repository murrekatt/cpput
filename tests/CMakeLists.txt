set(GCOV_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "-Wall -W -Werror -pedantic -O0 -fno-inline ${GCOV_FLAGS}")

set(SRCS
  Test_TestHarness.cpp
  main.cpp
)

add_executable(unittests ${SRCS})
add_test(unittests ${PROJECT_BINARY_DIR}/tests/unittests)