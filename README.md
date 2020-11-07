# UnitTest

A minimal, header-only unit-testing framework for C++ projects.

## Features

The project defines the following basic *MACROS* to construct test cases.

| MACROS | Description |
| :---- | :---- |
| `TEST(TestSuite, TestName)` | Declares test cases. |
| `BEGIN_TEST(TestSuite, TestName)` | Construct a testing environment. |
| `END_TEST` | Tear-down the test case. |
| `RUN_TEST(TestSuite, TestName)` | Calls the test case to execute. |
| `EXPECT_EQ(arg1, arg2)` | Checks if two arguments are equal. |

### Additional Macros

| MACROS | Description |
| :---- | :---- |
|`INIT_TEST_SUMMARY` | Initializates variables to store the number of passed and failed test cases. <br> (Mandatory) |
| `RUN_TEST_SUMMARY` | Calculates and prints the total number of passed and failed test cases. <br> (Optional)|
| `TEST_STATUS` | `0` if all tests pass. Else, `-1`. <br> (For use when running with CTest.) |

## Sample Usage

Here's how you can use UnitTest in your project.

- Pull the repository as a submodule in your project. <br>
`git submodule add git@gitlab.com:AvinashRavishankar/unittest.git
./test/unittest`

- Create a header file to declare your tests.
```cpp
//sample_test.hpp

TEST(Suite1, Name1)
TEST(Suite2, Name2)
...
```

- Create a source file to define all your test cases.
```cpp
// sample_test.cpp

BEGIN_TEST(Suite1, Name1){
   // Initializations
   // Call the function under test
   EXPECT_EQ(valueReturned, valueExpected)
}END_TEST

BEGIN_TEST(Suite2, Name2){
   // Initializations
   // Call the function under test
   EXPECT_EQ(valueReturned, valueExpected)
}END_TEST

...
```
- Create a source file to run all tests.
```cpp
// test_main.cpp

#include "unittest/unittest.hpp"
#include "sample_test.hpp"

INIT_TEST_SUMMARY // Should be defined in the main test source.

int main(){

   RUN_TEST(Suite1, Name1)
   RUN_TEST(Suite2, Name2)
   ...

   RUN_TEST_SUMMARY // Prints test statistics (optional)

   return TEST_STATUS; /** TEST_STATUS should be used when running tests with
                         CTest. Otherwise, the progam may return -1 on
                         successful execution.

                         Use `return 0` otherwise.
                       */
}
```
## Thread Safety

UnitTest allows *forking* test cases to run them in separate threads. This
way, when a test case crashes, it dosen't bring down the entire program.

Forking is disabled by default. To enable it, simply append `-DFORK` to your
compiler options. No  more changes! :relaxed:



