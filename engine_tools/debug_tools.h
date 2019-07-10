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
 * You can report errors in 2 forms:
 *    1. `uError_v` (Verbose): Print a formatted error with file
 *        and line information to stderr.
 *    2. `uError`: Print a formatted error without file and line
 *        information to stderr.
 *
 * The length of the message is determined by the `MAX_ERROR_LEN` macro
 * set in the lines below. Neither of these exit the process.
 */
#if __uDEBUG_SYSTEM__
#define MAX_ERROR_LEN 150
char _error_buffer[MAX_ERROR_LEN];
char _message_buffer[MAX_ERROR_LEN];

#define uError_v(...)                                     \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ ERROR::%s::%d ] %s",                            \
             __FILE__,                                          \
             __LINE__,                                          \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr)

#define uError(...)                                       \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ ERROR ] %s",                                    \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr)
#endif // __uDEBUG_SYSTEM__

#endif // __debug_tools
