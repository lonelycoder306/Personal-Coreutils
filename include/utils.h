#ifndef UTILS_H
#define UTILS_H

/* 
*  Defines any useful functions or function-like macros that we
*  use throughout the project.
*/

#include "common.h"
#include <string.h>

#define MATCH(str, short, long) \
    (!strcmp(str, short) || !strcmp(str, long))
#define SET_BIT(f, bit)     ((f) |= (bit))
#define RESET_BIT(f, bit)   ((f) &= ~(bit))

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202000L)
    #define NORETURN    [[noreturn]]
#else
    #include <stdnoreturn.h>
    #define NORETURN    noreturn
#endif

// Internal issues.
void    end(const char* util_name, const char* src,
    const char* msg);
// Command misuse.
void    error(const char* util_name, int exit_code,
    const char* msg, ...);
char*   p_getcwd();
void    display_version(const char* util_name);
void    display_help(help_info* info);
u64     set_bitflags(cl_opt opts[], char* argv[], int count, int* skip);

#endif