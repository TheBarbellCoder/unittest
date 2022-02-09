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
    Log(const char* module, const char* test, bool state)
    {
        std::stringstream sUnit;
        sUnit.flags(std::ios_base::left);
        sUnit << "( " << module << " ) " << test << "   ";

        _sFailed.flags(std::ios_base::right);
        _sFailed.fill('.');
        _sFailed.width(50);

        _sPassed.flags(std::ios_base::right);
        _sPassed.fill('.');
        _sPassed.width(50);

#ifdef __unix__
        _sFailed << "\x1b[38;5;160m   FAILED \x1b[0m \n";
        _sPassed << "\x1b[38;5;40m   OK \x1b[0m \n";
#else
        _sFailed << "   FAILED \n";
        _sPassed << "   OK \n";
#endif

        if(state) { _stream << sUnit.str() + _sPassed.str(); }
        else { _stream << sUnit.str() << _sFailed.str(); }
    }

    ~Log() { std::cerr << _stream.str() << std::endl; }
};

class Summary
{
    std::stringstream _sBorder;
    std::stringstream _sTotal;
    std::stringstream _sPassed;
    std::stringstream _sFailed;

    int total;
    public:
        Summary(const int& passed, const int& failed):
            total(passed + failed)
        {
            _sBorder.fill('=');
            _sBorder.width(80);
            _sBorder << "\n";

            _sPassed << "Tests Passed : " << "\x1b[38;5;40m"
                << passed << "\x1b[0m";
            _sFailed << "Tests Failed : " << "\x1b[38;5;160m"
                << failed << "\x1b[0m";

            _sTotal << "Number of tests: ";
        }

        ~Summary()
        {
            std::cerr << "\n" << _sBorder.str() <<  _sTotal.str()
                << total << "\n" << _sPassed.str() << "\n" << _sFailed.str()
                << "\n" << _sBorder.str() << std::endl;
        }
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

        static UnitTest& getInstance()
        {
            static UnitTest uTest;
            return uTest;
        }

        void runTests()
        {
            static int passed{0};
            static int failed{0};

            if(testList.empty())
            {
                std::cout << "No tests registered!!" << std::endl;
                return;
            }

            for(UnitTest* x: testList)
            {
#if  defined(__unix__) && defined(FORK)

                pid_t pid = vfork();

                if(pid < 0)
                {
                    std::cerr << "\n" << "In function " << __func__ << ": /n"
                        << "\x1b[38;5;160mFailed to create a child process!!\x1b[0m"
                        << std::endl;
                }

                if(pid > 0)
                {
                    int status;
                    waitpid(pid, &status, WCONTINUED);
                    if(WIFEXITED(status) == 0)
                    {
                        std::cerr << "\n" << "In function " << __func__ << ": \n"
                            << "\x1b[38;5;160mFailed with error code " <<
                            WEXITSTATUS(status) << "\x1b[0m" << std::endl;
                    }
                }

                if(pid == 0)
                {
                    x->runFunc();
                    x->isTrue ? ++passed : ++failed;
                    Log(x->moduleName, x->testName, x->isTrue);
                    _exit(0);
                }
#else
                x->runFunc();
                x->isTrue ? ++passed : ++failed;
                Log(x->moduleName, x->testName, x->isTrue);

#endif // __unix__ && FORK
            }
            Summary(passed, failed);
        }

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

#define ExpectEQ(arg1, arg2)                                                 \
    isTrue &= expectEQ(arg1, arg2);

#define DefineTest(Module, TestName)                                          \
    class Test_##Module##_##TestName: public UnitTest                         \
{                                                                             \
    Test_##Module##_##TestName(): UnitTest(#Module, #TestName){}              \
                                                                              \
    public:                                                                   \
                                                                              \
    ~Test_##Module##_##TestName() {}                                          \
                                                                              \
    static Test_##Module##_##TestName& getInstance()                          \
    {                                                                         \
        static Test_##Module##_##TestName newTestUnit;                        \
        return newTestUnit;                                                   \
    }                                                                         \
    void runFunc() override;                                                  \
};                                                                            \
                                                                              \
void Test_##Module##_##TestName::runFunc()

#define RegisterTest(Module, TestName)                                        \
    UnitTest::getInstance().testList.push_back(&(Test_##Module##_##TestName::getInstance()));

#define RunTests()                                                            \
    UnitTest::getInstance().runTests();

#endif
