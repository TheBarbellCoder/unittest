# UnitTest

__A minimal unit-testing library for C++__ 

## Features

The project defines the following basic *5 MACROS* to construct test cases.

| MACROS | Description |
| :---- | :---- |
| `DeclareTest(TestSuite, TestName)` | Declare test cases |
| `DefineTest(TestSuite, TestName)` | Define test cases |
| `RegisterTest(TestSuite, TestName)` | Register test cases to be run |
| `RunTests` | Executes all registerd tests |
| `ExpectEQ(arg1, arg2)` | Checks if two arguments are equal |

## Importing `Unittest` to Your Project

- Method 1 
    1. Pull the repository as a submodule in your project
    ```git
    git submodule add https://github.com/TheBarbellCoder/unittest.git ./test/unittest
    ```
    2. Add the following lines in CMakeLists.txt of your project
    ```cmake
    add_subdirectory(test/unittest)
    ...

    target_include_directories(<your_project> PRIVATE test/unittest/includes)
    target_link_libraries(<your_project> unittest)
    ```

- Method 2
    1. Import repository directly into your project using the `FetchContent` module from cmake
    ```cmake
    include(FetchContent)
    FetchContent_Declare(Unittest 
            GIT_REPOSITORY https://github.com/TheBarbellCoder/unittest.git)
    FetchContent_MakeAvailable(Unittest)
    ```
    2. Repeat steps 2 and 3 from _Method 1_
    ```cmake
    add_subdirectory(${unittest_SOURCE_DIR} ${uniitest_BINARY_DIR})
    ...

    target_include_directories(<your_project> PRIVATE ${unittest_SOURCE_DIR}/includes)
    target_link_libraries(<your_project> ${unittest_BINARY_DIR}/unittest)
    ```

## Sample Usage

Here's how you'd use `UnitTest` in your project.
```cpp
// File: test_main.cpp
#include "test_project.hpp"

int main(){
    RegisterTest(module, test1)
    RegisterTest(module, test2)

    RunTests()

    return 0;
}
```
```cpp
// File: test_project.cpp
#include "test_project.hpp"

DefineTest(module, test1){

    // Your code here
    // ...
    // ExpectEQ(<actual>, <expected>)
}

DefineTest(module, test2){
    
    // Your code here
    // ...
    // ExpectEQ(<actual>, <expected>)
}
```
```cpp
// File: test_project.hpp
#include "unittest.hpp"

DeclareTest(module, test1)
DeclareTest(module, test2)
```

## Running Tests in Isolated Processes

UnitTest allows *forking* test cases to run them in separate processes. This way, when a test case crashes, due to an unhandled exception or segfault, it dosen't bring down the entire program.

Forking is disabled by default and is currently available to _*nix_ systems. To enable it, simply define `FORK` in your main test source file or append `-DFORK` to your CMake command! :relaxed:
