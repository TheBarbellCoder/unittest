# UnitTest

__A minimal unit-testing framework for C++__ 

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
```
1. Pull the repository as a submodule in your project
`git submodule add https://github.com/TheBarbellCoder/unittest.git ./test/unittest`

2. Add `./test/unittest` as a subdirectory in the root CMakeLists.txt in your project
`add_subdirectory("test/unittest")`

3. Build it with your CMake project
```
- Method 2
```
1. Import repository directly into your project using CMake's `FetchContent` module
2. Add the repo as a subdirectory with `add_subdirectory` the your project's root CMakeLists.txt
3. Build as usual
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
/ File: test_project.cpp
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
