#ifndef MY_TEST_MACROS
#define MY_TEST_MACROS

#include <stdio.h>
#include <stdlib.h>

#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BOLD    "\x1B[1m"

#define TEST(test_name)                                     \
   do {                                                     \
      if (test_name()){                                     \
        printf(RED BOLD "  %s FAILED\n" RESET, #test_name);   \
      } else {                                              \
        printf(GREEN BOLD "  %s PASSED\n" RESET, #test_name); \
      }                                                     \
   }while(0)                                                \
      
#define TEST_GROUP(group)                                 \
   do {                                                   \
      printf(YELLOW BOLD "TESTING: %s\n" RESET, #group);   \
      group();                                            \
   }while(0)                                              \
      

#define ASSERT(cond, msg)                                      \
    do {                                                       \
        if (!(cond)) {                                         \
            fprintf(stderr, RED BOLD "  Assertion failed: %s\n"  \
                    "  File: %s, Line: %d\n"                     \
                    "  Message: %s\n" RESET,                     \
                    #cond, __FILE__, __LINE__, msg);           \
            return 1;                                          \
        }                                                      \
    } while (0)

#endif // !MACROS
