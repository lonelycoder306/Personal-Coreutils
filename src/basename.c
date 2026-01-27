/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "basename"

typedef u8 flag;
#define DEF_FLAG (flag) 0x0

#define LETTER_OPTS 3 // -a, -s, -z

#if defined(_WIN32) || defined(_WIN64)
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif

/* 5 options for basename. */

/* Option bits. */

#define HELP_BIT        (1 << 7)
#define VERSION_BIT     (1 << 6)
#define MULTIPLE_BIT    (1 << 5)
#define SUFFIX_BIT      (1 << 4)
#define ZERO_BIT        (1 << 3)

/* Option bit-masks. */

#define HELP(f)     ((f) & HELP_BIT)
#define VERSION(f)  ((f) & VERSION_BIT)
#define MULTIPLE(f) ((f) & MULTIPLE_BIT)
#define SUFFIX(f)   ((f) & SUFFIX_BIT)
#define ZERO(f)     ((f) & ZERO_BIT)

/* Help option data. */

/* Helpers. */

static flag
set_flags(int argc, char* argv[], int* skip)
{
    flag f = DEF_FLAG;
    
    if (argc < 2) return f;

    if (!strcmp(argv[1], "--help") && (argc == 2))
        SET_BIT(f, HELP_BIT);
    else if (!strcmp(argv[1], "--version") && (argc == 2))
        SET_BIT(f, VERSION_BIT);
    else
    {
        cl_opt opts[LETTER_OPTS]  = {
            {'a', "--multiple", MULTIPLE_BIT, SET},
            {'s', "--suffix", SUFFIX_BIT, SET},
            {'z', "--zero", ZERO_BIT, SET}
        };
        f = (flag) set_bitflags(opts, argv, LETTER_OPTS, skip);
    }

    return f;
}

static char*
find_sep(const char* path, size_t len)
{
    char* pos = NULL;
    if (path[len - 1] == PATH_SEP)
        pos = (char *) path + len - 1;
    for (size_t i = 0; i <= len - 2; i++)
    {
        size_t index = len - 2 - i;
        if (path[index] == PATH_SEP)
            return (char *) path + index;
    }

    return pos;
}

static char*
substr(const char* s, size_t len)
{
    char* ret = calloc(len + 1, sizeof(char));
    if (ret == NULL)
        end(UTIL_NAME, "substr", "Allocation failure.");
    memcpy(ret, s, len);
    return ret;
}

static char*
get_basename(const char* path)
{
    size_t len = strlen(path);
    char* pos = find_sep(path, len);
    if (pos == NULL)
        return substr(path, len);
    if (pos[1] == '\0') // Separator is at the end.
        return substr(path, len - 1);
    return substr(pos + 1, len - (pos - path) - 1);
}

/* Main. */

int
main(int argc, char* argv[])
{
    int skip = 1;
    flag f = set_flags(argc, argv, &skip);

    if (HELP(f)) {}
    else if (VERSION(f))
    {
        display_version(UTIL_NAME);
        exit(EXIT_SUCCESS);
    }

    for (int i = skip; i < argc; i++)
    {
        char* print = get_basename(argv[i]);
        printf("%s%s", print, ZERO(f) ? "" : "\n");
        free(print);
        if (!MULTIPLE(f)) break;
    }

    return 0;
}