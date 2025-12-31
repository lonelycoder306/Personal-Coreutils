/* Includes. */

#include "../include/utils.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "cat"
typedef uint8_t flag;
#define DEF_FLAG (flag) 0x0

/* 11 options for cat. */
/* 8 unique options. */

/* Option bits. */

#define HELP_BIT        7
#define VERSION_BIT     6
#define NO_BLANK_BIT    5
#define SHOW_ENDS_BIT   4
#define NUM_LINES_BIT   3
#define SQUEEZE_BIT     2
#define TAB_BIT         1
#define NONPRINT_BIT    0

/* Option bit-masks. */

#define HELP(f)         (((f) >> HELP_BIT) & 0x1)
#define VERSION(f)      (((f) >> VERSION_BIT) & 0x1)
#define NO_BLANK(f)     (((f) >> NO_BLANK_BIT) & 0x1)
#define SHOW_ENDS(f)    (((f) >> SHOW_ENDS_BIT) & 0x1)
#define NUM_LINES(f)    (((f) >> NUM_LINES_BIT) & 0x1)
#define SQUEEZE(f)      (((f) >> SQUEEZE_BIT) & 0x1)
#define NONPRINT(f)     (((f) >> NONPRINT_BIT) & 0x1)

/* Help option data. */

static const opt_pair
cat_options[] = {
    {NULL, NULL}
};

static const char*
cat_option_details[] = {
    NULL
};

static const char*
cat_notes[] = {
    NULL
};

/* Helpers. */

static flag
set_flags(int argc, char* argv[])
{
    flag f = DEF_FLAG;

    if (argc < 2)
        return f;

    if (!strcmp(argv[1], "--help"))
        SET_BIT(f, HELP_BIT);
    else if (!strcmp(argv[1], "--version"))
        SET_BIT(f, VERSION_BIT);
    else if (MATCH(argv[1], "-b", "--number-nonblank"))
        SET_BIT(f, NO_BLANK_BIT);
    else if (MATCH(argv[1], "-E", "--show-ends"))
        SET_BIT(f, SHOW_ENDS_BIT);
    else if (MATCH(argv[1], "-n", "--number"))
        SET_BIT(f, NUM_LINES_BIT);
    else if (MATCH(argv[1], "-s", "--squeeze-blank"))
        SET_BIT(f, SQUEEZE_BIT);
    else if (MATCH(argv[1], "-T", "--show-tabs"))
        SET_BIT(f, TAB_BIT);
    else if (MATCH(argv[1], "-v", "--show-nonprinting"))
        SET_BIT(f, NONPRINT_BIT);

    return f;
}

static void
help_option()
{
    help_info info = {
        "cat [OPTION]... [FILE]...",
        "Concatenate FILE(s) to standard output.",
        cat_options,
        cat_option_details,
        cat_notes,
    };

    display_help(&info);
}

int main(int argc, char *argv[])
{
    flag f = set_flags(argc, argv);
    int skip = 1;
    if (f != DEF_FLAG)
        skip = 2;

    if (HELP(f))
    {
        if (argc == 2)
        {
            help_option();
            exit(EXIT_SUCCESS);
        }
        else
            skip--;
    }
    else if (VERSION(f))
    {
        if (argc == 2)
        {
            display_version(UTIL_NAME);
            exit(EXIT_SUCCESS);
        }
        else
            skip--;
    }
    
    return 0;
}