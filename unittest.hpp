#ifndef TEST_INCLUDED
#define TEST_INCLUDED

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>

struct TestSummary{
   static int True;
   static int False;
};

#define INIT_TEST_SUMMARY                                                     \
   int TestSummary::True = 0;                                                 \
   int TestSummary::False = 0;

#define EXPECT_EQ(arg1, arg2) isTrue &= (arg1 == arg2);

#define TEST_STATUS (TestSummary::False > 0) ? -1 : 0;

#define TEST(TestSuite, TestName) bool test_##TestSuite##_##TestName(void);

#ifdef FORK

#define BEGIN_TEST(TestSuite, TestName)                                       \
   bool test_##TestSuite##_##TestName(void)                                   \
{                                                                             \
      bool ret;                                                               \
      pid_t pid = fork();                                                     \
                                                                              \
      if(pid < 0)                                                             \
      {                                                                       \
        std::cout                                                             \
        << "\n" << "In function " << __func__ << ": \n"                       \
        << "\x1b[38;5;160mFailed to create a child process!!\x1b[0m"          \
        << std::endl;                                                         \
        ret = false;                                                          \
      }                                                                       \
      if(pid > 0)                                                             \
      {                                                                       \
        int status;                                                           \
        waitpid(pid, &status, 0);                                             \
        if(WIFEXITED(status))                                                 \
        {                                                                     \
          ret = (WEXITSTATUS(status) == EXIT_SUCCESS) ? true : false;         \
        }                                                                     \
        else                                                                  \
        {                                                                     \
          std::cout                                                           \
          << "\n" << "In function " << __func__ << ": \n"                     \
          << "\x1b[38;5;160mFailed to exit normally!!\x1b[0m"                 \
          << std::endl;                                                       \
          ret = false;                                                        \
        }                                                                     \
      }                                                                       \
      if(pid == 0)                                                            \
      {                                                                       \
        bool isTrue{true};

#define END_TEST                                                              \
        exit(isTrue ? EXIT_SUCCESS : !EXIT_SUCCESS);                          \
      }                                                                       \
      return ret;                                                             \
}

#else

#define BEGIN_TEST(module, name)                                              \
   bool test__##module##__##name(void)                                        \
{                                                                             \
   bool isTrue{true};

#define END_TEST                                                              \
   return isTrue;                                                             \
}

#endif

#define RUN_TEST(module, name)                                                \
{                                                                             \
                                                                              \
   bool ret = test_##module##_##name();                                       \
   std::cout << std::left << std::setfill('-')                                \
   << std::setw(50) << #module " -> " #name " ";                              \
                                                                              \
   if(ret)                                                                    \
   {                                                                          \
      TestSummary::True += 1;                                                 \
      std::cout << std::setw(10)                                              \
      << std::left << "\x1b[38;5;40m   OK \x1b[0m" << "\n" << std::endl;      \
   }                                                                          \
   else                                                                       \
   {                                                                          \
      TestSummary::False += 1;                                                \
      std::cout << std::setw(10)                                              \
      << std::left << "\x1b[38;5;160m   FAILED \x1b[0m" << "\n" << std::endl; \
   }                                                                          \
}

#define RUN_TEST_SUMMARY                                                      \
   std::cout << std::setw(80) << std::left << std::setfill('-') << "\n"       \
   << std::endl;                                                              \
   std::cout << "Tests Passed : " << "\x1b[38;5;40m" << TestSummary::True     \
   << "\x1b[0m" << std::endl;                                                 \
                                                                              \
   std::cout << "Tests Failed : " << "\x1b[38;5;160m" << TestSummary::False   \
   << "\x1b[0m" << std::endl;                                                 \
   std::cout << std::left << std::setfill('-') << std::setw(80) << std::endl;

#endif
