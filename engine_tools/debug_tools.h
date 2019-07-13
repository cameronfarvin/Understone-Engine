#ifndef __debug_tools__
#define __debug_tools__ 1

#include <string.h>

// Set to 0 to disable all debugging functions
#define __uDEBUG_SYSTEM__ 1

#ifndef functionFired
#if __uDEBUG_SYSTEM__
#define functionFired  printf("[ debug ] function fired: %s\n", __func__);
#else
#define functionFired  /* Debugging is disabled: functionFired */
#endif // __uDEBUG_SYSTEM__
#endif // functionFired

/*
 * Understone Error Reporting
 *
 * Error reporting:
 *    1. `uError_v` (Verbose): Print a formatted error with file
 *        and line information to stderr (does not halt).
 *    2. `uError`: Print a formatted error without file and line
 *        information to stderr (does not halt).
 *
 * Debug printing:
 *    1. `uDBGPrint_v` (Verbose): Print a formatted debug string
 *        with file and line information to stderr. (does not
 *        halt).
 *    2. `uDBGPrint`: Print a formatted debug string without
 *        file and line information to stderr. (does not halt).
 *
 * Debug assertions:
 *    1. `uDBGAssert_v` (Verbose): If the condition is false,
 *        print a message with file and line information to
 *        stderr and exit the process with code 666.
 *    1. `uDBGAssert` (Verbose): If the condition is false,
 *        print a message without file and line information to
 *        stderr and exit the process with code 666.
 *
 * The length of the message/error is determined by the
 * `MAX_ERROR_LEN` macro set in the lines below.
 *
 */
#if __uDEBUG_SYSTEM__
#define MAX_ERROR_LEN 150
char _error_buffer[MAX_ERROR_LEN];
char _message_buffer[MAX_ERROR_LEN];

#define uError_v(...)                                           \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ error::%s::%d ] %s",                            \
             __FILE__,                                          \
             __LINE__,                                          \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr);                               \
    fflush(stderr)

#define uError(...)                                             \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ error ] %s",                                    \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr)

#define uDBGPrint_v(...)                                        \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ debug::%s::%d ] %s",                            \
             __FILE__,                                          \
             __LINE__,                                          \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr);                               \
    fflush(stderr)

#define uDBGPrint(...)                                          \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ debug ] %s",                                    \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr);                               \
    fflush(stderr)

#define uDBGAssertMsg(cond, ...)                                \
    if (!cond) {                                                \
        snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);  \
        snprintf(_error_buffer,                                 \
                 MAX_ERROR_LEN,                                 \
                 "[ assertion ] %s",                            \
                 _message_buffer);                              \
        fputs(_error_buffer, stderr);                           \
        fflush(stderr);                                         \
        exit(666); }

#define uDBGAssertMsg_v(cond, ...)                              \
    if (!(cond)) {                                              \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ assertion::%s::%d ] %s",                        \
             __FILE__,                                          \
             __LINE__,                                          \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr);                               \
    fflush(stderr);                                             \
    exit(666); }

#define uDBGAssert(cond)                        \
    if (!((long long) (cond))) { exit(666); }   \

// [ cfarvin::TODO ] [ cfarvin::IMPLEMENT ]
// Implement a warning(s) level debug message.
// Example of needing such a thing would be incomplete or
// erroneous frees/mallocs that should not print for debug
// nor crash the engine.



#else // __uDEBUG_SYSTEM__
#define uError_v(...) /* System debugging is disabled */
#define uError(...) /* System debugging is disabled */
#define uDBGPrint_v(...) /* System debugging is disabled */
#define uDBGPrint(...) /* System debugging is disabled */
#define uDBGAssert(cond, ...) /* System debugging is disabled */
#define uDBGAssert_v(cond, ...) /* System debugging is disabled */
#endif // __uDEBUG_SYSTEM__



#endif // __debug_tools
