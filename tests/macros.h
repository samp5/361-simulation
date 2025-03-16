#ifndef MY_TEST_MACROS
#define MY_TEST_MACROS

#include <stdio.h>
#include <stdlib.h>

extern int WOULD_FAIL;
static char buf[1024];

#ifndef COLORS
#define COLORS

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BOLD "\x1B[1m"

#endif // !COLORS

#define TEST_CONTEXT_F(msg, ...)                                               \
  do {                                                                         \
    printf(GREEN BOLD msg RESET, __VA_ARGS__);                                 \
  } while (0)

#define TEST_CONTEXT(msg)                                                      \
  do {                                                                         \
    printf(GREEN BOLD "  %s" RESET, msg);                                      \
  } while (0)

#define TEST(test_name)                                                        \
  do {                                                                         \
    if (test_name()) {                                                         \
      printf(RED BOLD "  %s FAILED\n" RESET, #test_name);                      \
    } else {                                                                   \
      printf(GREEN BOLD "  %s PASSED\n" RESET, #test_name);                    \
    }                                                                          \
  } while (0)

#define TEST_GROUP(group)                                                      \
  do {                                                                         \
    printf(YELLOW BOLD "TESTING: %s\n" RESET, #group);                         \
    group();                                                                   \
  } while (0)

#define TEST_SUBGROUP(group)                                                   \
  do {                                                                         \
    printf(YELLOW BOLD "  TESTING: %s\n" RESET, #group);                       \
    group();                                                                   \
  } while (0)

#define TEST_INFO(msg, ...)                                                    \
  do {                                                                         \
    sprintf(buf, msg, __VA_ARGS__);                                            \
    printf(YELLOW BOLD "%s\n" RESET, buf);                                     \
  } while (0)

#define ASSERT(cond, msg)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr,                                                          \
              RED BOLD "  Assertion failed: %s\n"                              \
                       "  File: %s, Line: %d\n"                                \
                       "  Message: %s\n" RESET,                                \
              #cond, __FILE__, __LINE__, msg);                                 \
      return 1;                                                                \
    }                                                                          \
  } while (0)

#define ASSERT_FAIL(msg, func, ...)                                            \
  do {                                                                         \
    func(__VA_ARGS__);                                                         \
    if (!WOULD_FAIL) {                                                         \
      fprintf(stderr,                                                          \
              RED BOLD "  %s should fail\n"                                    \
                       "  File: %s, Line: %d\n"                                \
                       "  Message: %s\n" RESET,                                \
              #func, __FILE__, __LINE__, msg);                                 \
      return 1;                                                                \
    } else {                                                                   \
      reset();                                                                 \
    }                                                                          \
  } while (0)

#define ASSERT_VALID(msg, func, ...)                                           \
  do {                                                                         \
    func(__VA_ARGS__);                                                         \
    if (WOULD_FAIL) {                                                          \
      fprintf(stderr,                                                          \
              RED BOLD "  %s should not fail\n"                                \
                       "  File: %s, Line: %d\n"                                \
                       "  Message: %s\n" RESET,                                \
              #func, __FILE__, __LINE__, msg);                                 \
      return 1;                                                                \
    } else {                                                                   \
      reset();                                                                 \
    }                                                                          \
  } while (0)

#define FASSERT(cond, msg, ...)                                                \
  do {                                                                         \
    if (!(cond)) {                                                             \
      sprintf(buf, msg, __VA_ARGS__);                                          \
      fprintf(stderr,                                                          \
              RED BOLD "  Assertion failed: %s\n"                              \
                       "  File: %s, Line: %d\n"                                \
                       "  Message: %s\n" RESET,                                \
              #cond, __FILE__, __LINE__, buf);                                 \
      return 1;                                                                \
    }                                                                          \
  } while (0)
#endif // !MACROS
