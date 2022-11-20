#ifndef TEST_INCLUDED
#define TEST_INCLUDED

#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <list>


class Log
{
    std::stringstream _stream;
    std::stringstream _sFailed;
    std::stringstream _sPassed;

    public:
    Log(const char* module, const char* test, bool state);
    ~Log();
};


class Summary
{
    std::stringstream _sBorder;
    std::stringstream _sTotal;
    std::stringstream _sPassed;
    std::stringstream _sFailed;

    int total;
    public:
        Summary(const int& passed, const int& failed);
        ~Summary();
};


class UnitTest
{
    UnitTest() {}

    protected:

        UnitTest(const char* module, const char* test):
            moduleName(module), testName(test) {}

    public:

        const char* moduleName;
        const char* testName;
        bool isTrue{true};
        std::list<UnitTest*> testList;

        static UnitTest& getInstance();

        void runTests();

        template<typename T>
        inline bool expectEQ(const T& arg1, const T& arg2)
        {
            bool isTrue{arg1 == arg2};
            if(!isTrue)
            {
                std::cout << "Actual: " << arg1 << " vs. Expected: " << arg2
                    << std::endl;
            }
            return isTrue;
        }

        virtual void runFunc() {}
        virtual ~UnitTest() {}
};


#define DeclareTest(Module, TestName)                                          \
    class Test_##Module##_##TestName: public UnitTest                          \
{                                                                              \
    Test_##Module##_##TestName(): UnitTest(#Module, #TestName){}               \
    public:                                                                    \
        static Test_##Module##_##TestName* getInstance()                       \
    {                                                                          \
        static Test_##Module##_##TestName testClass;                           \
        return &testClass;                                                     \
    }                                                                          \
        void runFunc() override;                                               \
};                                                                             \


#define DefineTest(Module, TestName)                                           \
void Test_##Module##_##TestName::runFunc()


#define RegisterTest(Module, TestName)                                         \
    UnitTest::getInstance().testList.push_back(                                \
            Test_##Module##_##TestName::getInstance());


#define ExpectEQ(arg1, arg2)                                                   \
    isTrue &= expectEQ(arg1, arg2);


#define RunTests()                                                             \
    UnitTest::getInstance().runTests();

#endif // TEST_INCLUDED
