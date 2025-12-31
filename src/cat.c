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

#define LETTER_OPTS 10 // -A, -b, -e, -E, -n, -s, -t, -T, -u, -v

/* Option bits. */

#define HELP_BIT        (1 << 7)
#define VERSION_BIT     (1 << 6)
#define NO_BLANK_BIT    (1 << 5)
#define SHOW_ENDS_BIT   (1 << 4)
#define NUM_LINES_BIT   (1 << 3)
#define SQUEEZE_BIT     (1 << 2)
#define TAB_BIT         (1 << 1)
#define NONPRINT_BIT    (1 << 0)

/* Option bit-masks. */

#define HELP(f)         ((f) & HELP_BIT)
#define VERSION(f)      ((f) & VERSION_BIT)
#define NO_BLANK(f)     ((f) & NO_BLANK_BIT)
#define SHOW_ENDS(f)    ((f) & SHOW_ENDS)
#define NUM_LINES(f)    ((f) & NUM_LINES_BIT)
#define SQUEEZE(f)      ((f) & SQUEEZE_BIT)
#define TAB(f)          ((f) & TAB_BIT)
#define NONPRINT(f)     ((f) & NONPRINT_BIT)

/* Help option data. */

static const opt_group
cat_options[] = {
    {"-A", "--show-all", ""},
    {"-b", "--number-nonblank",
        "Number all nonempty output lines. Overrides '-n'."},
    {"-e", NULL, ""},
    {"-E", "--show-ends", "Display '$' at the end of each line."},
    {"-n", "--number", "Number all output lines."},
    {"-s", "--squeeze-blank", "Suppress repeated empty output lines."},
    {"-t", NULL, ""},
    {"-T", "--show-tabs", "Display TAB (\\t) characters as ^I."},
    {"-u", NULL, "(ignored)"},
    {"-v", "--show-nonprinting", "Replace non-printable characters using ^ and "
        "M- notation, except for LFD and TAB."},
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
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
set_flags(int argc, char* argv[], int* skip)
{
    flag f = DEF_FLAG;

    if (argc < 2)
        return f;

    if (!strcmp(argv[1], "--help") && (argc == 2))
        SET_BIT(f, HELP_BIT);
    else if (!strcmp(argv[1], "--version") && (argc == 2))
        SET_BIT(f, VERSION_BIT);
    else
    {
        cl_opt opts[LETTER_OPTS] = {
            {'A', "--show-all", NONPRINT_BIT | SHOW_ENDS_BIT | TAB_BIT},
            {'b', "--number-nonblank", NO_BLANK_BIT},
            {'e', NULL, NONPRINT_BIT | SHOW_ENDS_BIT},
            {'E', "--show-ends", SHOW_ENDS_BIT},
            {'n', "--number", NUM_LINES_BIT},
            {'s', "--squeeze-blank", SQUEEZE_BIT},
            {'t', NULL, NONPRINT_BIT | TAB_BIT},
            {'T', "--show-tabs", TAB_BIT},
            {'u', NULL, 0},
            {'v', "--show-non-printing", NONPRINT_BIT}
        };
        f = set_bitflags(opts, argv, LETTER_OPTS, skip);
    }

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

int
main(int argc, char *argv[])
{
    int skip = 1;
    flag f = set_flags(argc, argv, &skip);

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
    
    return 0;
}