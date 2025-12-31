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

void    end(char* src, char* msg);
char*   p_getcwd();
void    display_version(const char* util_name);
void    display_help(help_info* info);
int     set_bitflags(char opts[], char* argv[], int flags[],
    int count, int* skip);

#endif