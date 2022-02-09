# UnitTest

A minimal, header-only unit-testing framework for C++ projects.

## Features

The project defines the following basic *MACROS* to construct test cases.

| MACROS | Description |
| :---- | :---- |
| `DefineTest(TestSuite, TestName)` | Define and Declare test cases |
| `RegisterTest(TestSuite, TestName)` | Register test cases to be run |
| `RunTests` | Executes all registerd tests |
| `ExpectEQ(arg1, arg2)` | Checks if two arguments are equal |

## Sample Usage

Here's how you can use UnitTest in your project.

- Pull the repository as a submodule in your project. <br>
`git submodule add git@gitlab.com:AvinashRavishankar/unittest.git
./test/unittest`

- Create a source file to declare and define your test cases.
```cpp
//sample_test.cpp

DefineTest(Suite1, Name1)
{
   // Define your test case here
}
DefineTest(Suite2, Name2)
{
   // Define your test case here
}
...
```

- Create a source file to register and run all tests.
```cpp
// test_main.cpp

#include "unittest/unittest.hpp"
#include "sample_test.hpp"

int main(){

   RegisterTest(Suite1, Name1)
   RegisterTest(Suite2, Name2)
   ...
   ...

   RunTests()

   return 0;
}
```
## Thread Safety

UnitTest allows *forking* test cases to run them in separate threads. This
way, when a test case crashes, it dosen't bring down the entire program.

Forking is disabled by default and is currently available to _unix-like_ OS. To enable it, simply define `FORK` in your main test source file or append `-DFORK` to your compile command! :relaxed:
