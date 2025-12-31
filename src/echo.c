/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "echo"
typedef uint8_t flag;
#define DEF_FLAG (flag) 0x0

/* 5 options for echo. */

/* Option bits. */

#define HELP_BIT        7
#define VERSION_BIT     6
#define NL_BIT          5
#define INTER_BIT       4
#define NO_INTER_BIT    3

/* Option bit-masks. */

#define HELP(f)     (((f) >> HELP_BIT) & 0x1)
#define VERSION(f)  (((f) >> VERSION_BIT) & 0x1)
#define NO_NL(f)    (((f) >> NL_BIT) & 0x1)
#define INTER(f)    (((f) >> INTER_BIT) & 0x1)
#define NO_INTER(f) (((f) >> NO_INTER_BIT) & 0x1)

/* Help option data. */

static const opt_pair
echo_options[] = {
    {"-n", "Do not output the trailing newline."},
    {"-e", "Enable interpretation of backslash escapes."},
    {"-E", "Disable interpretation of backslash escape (default)."},
    {"--help", "Display this help page and exit."},
    {"--version", "Output version information and exit."},
    {NULL, NULL}
};

static const char*
echo_option_details[] = {
    NULL
};

static const char*
echo_notes[] = {
    NULL
};

/* Helpers. */

static flag
set_flags(int argc, char* argv[])
{
    flag f = DEF_FLAG;
    
    if (argc < 2)
        return f;

    if (!strcmp(argv[1], "--help") && (argc == 2))
        SET_BIT(f, HELP_BIT);
    else if (!strcmp(argv[1], "--version") && (argc == 2))
        SET_BIT(f, VERSION_BIT);
    else if (!strcmp(argv[1], "-n"))
        SET_BIT(f, NL_BIT);
    else if (!strcmp(argv[1], "-e"))
        SET_BIT(f, INTER_BIT);
    else if (!strcmp(argv[1], "-E"))
        SET_BIT(f, NO_INTER_BIT);

    return f;
}

static void
help_option()
{
    help_info info = {
        "echo [OPTION]... [STRING]...",
        "Echo the STRING(s) to standard output.",
        echo_options,
        echo_option_details,
        echo_notes,
    };

    display_help(&info);
}

/* Main. */

int main(int argc, char *argv[])
{
    flag f = set_flags(argc, argv);
    int skip = 1;
    if (f != DEF_FLAG)
        skip = 2;

    if (HELP(f))
    {
        help_option();
        exit(EXIT_SUCCESS);
    }
    else if (VERSION(f))
    {
        display_version(UTIL_NAME);
        exit(EXIT_SUCCESS);
    }
    
    for (int i = skip; argv[i] != NULL; i++)
    {
        char* arg = argv[i];
        printf("%s", arg);
        if (argv[i + 1] != NULL) printf(" ");
    }

    if (!NO_NL(f))
        printf("\n");
    
    return 0;
}