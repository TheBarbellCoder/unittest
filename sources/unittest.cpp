#include "unittest.hpp"


Log::Log(const char* module, const char* test, bool state)
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


Log::~Log()
{
    std::cerr << _stream.str() << std::endl;
}


Summary::Summary(const int& passed, const int& failed):
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

Summary::~Summary()
{
    std::cerr << "\n" << _sBorder.str() <<  _sTotal.str()
        << total << "\n" << _sPassed.str() << "\n" << _sFailed.str()
        << "\n" << _sBorder.str() << std::endl;
}


UnitTest& UnitTest::getInstance()
{
    static UnitTest uTest;
    return uTest;
}

void UnitTest::runTests()
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
