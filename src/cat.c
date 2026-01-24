/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include "get_next_line.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "cat"
typedef u8 flag;
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
#define SHOW_ENDS(f)    ((f) & SHOW_ENDS_BIT)
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

    if (argc < 2) return f;

    if (!strcmp(argv[1], "--help") && (argc == 2))
        SET_BIT(f, HELP_BIT);
    else if (!strcmp(argv[1], "--version") && (argc == 2))
        SET_BIT(f, VERSION_BIT);
    else
    {
        cl_opt opts[LETTER_OPTS] = {
            {'A', "--show-all", NONPRINT_BIT | SHOW_ENDS_BIT | TAB_BIT, SET},
            {'b', "--number-nonblank", NO_BLANK_BIT, SET},
            {'e', NULL, NONPRINT_BIT | SHOW_ENDS_BIT, SET},
            {'E', "--show-ends", SHOW_ENDS_BIT, SET},
            {'n', "--number", NUM_LINES_BIT, SET},
            {'s', "--squeeze-blank", SQUEEZE_BIT, SET},
            {'t', NULL, NONPRINT_BIT | TAB_BIT, SET},
            {'T', "--show-tabs", TAB_BIT, SET},
            {'u', NULL, 0, SET},
            {'v', "--show-non-printing", NONPRINT_BIT, SET}
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

static inline char
control_char(char c)
{
    if ((c == '\n') || (c == '\t'))
        return c;
    return (c != 127 ? c + 0x40 : 0x3f);
}

#define IS_CTRL(c)  (((c) < 32) || ((c) == 127))
#define IS_META(c)  ((c) > 127)
#define AS_CTRL(c)  control_char(c)

static char*
format_str(char* line, flag f, size_t len)
{
    // We allocate more than enough space.
    char* fmt = calloc((4 * len) + 1, sizeof(char));
    if (fmt == NULL)
        end(UTIL_NAME, "format_str", "Allocation failure.");

    size_t i = 0;
    for (size_t j = 0; j < len; j++)
    {
        int c = line[j];
        if ((c > 31) && (c < 128))
            fmt[i++] = line[j];
        else if ((c == '\t') && TAB(f))
        {
            fmt[i++] = '^';
            fmt[i++] = 'I';
        }
        else if (IS_CTRL(c))
        {
            if ((c != '\n') && (c != '\t'))
                fmt[i++] = '^';
            fmt[i++] = AS_CTRL(c);
        }
        else if (IS_META(c))
        {
            fmt[i++] = 'M';
            fmt[i++] = '-';
            if (IS_CTRL(c & 0x7f))
            {
                fmt[i++] = '^';
                fmt[i++] = AS_CTRL(c & 0x7f);
            }
            else
                fmt[i++] = c & 0x7f;
        }
    }

    return fmt;
}

static int
open_file(char* filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        error(UTIL_NAME, -1,
            "File (\"%s\") could not be opened.", filename);
    
    return fd;
}

static void
print_file(int fd, flag f)
{
    char* line;
    int count = 1;
    bool prev_empty = false;
    size_t len;
    while ((line = get_next_line(fd, &len)) != NULL)
    {
        bool empty = ((len < 2) && (line[0] == '\n'));

        // Formatting for -T and -v options.

        if (TAB(f) || NONPRINT(f))
        {
            char* temp = line;
            line = format_str(line, f, len);
            free(temp);
        }

        // Checking for -s option.

        if (empty)
        {
            if (prev_empty)
                continue;
            else
                prev_empty = true;
        }
        else
            prev_empty = false;

        // Numbering for -b and -n options.

        if (NO_BLANK(f))
        {
            if (empty)
            {
                printf("%6d  ", count);
                count++;
            }
        }
        else if (NUM_LINES(f))
        {
            printf("%6d  ", count);
            count++;
        }

        // Formatting for -E option.

        bool newline = (len == 0 ? false : line[len - 1] == '\n');
        if (SHOW_ENDS(f))
        {
            if (newline)
            {
                printf("%.*s", (int) (len - 1), line);
                printf("$\n");
            }
            else
                printf("%s", line);
        }
        else
            printf("%s", line);
    }
}

int
main(int argc, char* argv[])
{
    if (argc == 1)
    {
        print_file(0, DEF_FLAG);
        exit(EXIT_SUCCESS);
    }
    
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

    int exit_code = 0;
    for (int i = skip; i < argc; i++)
    {
        if (!strcmp(argv[i], "-"))
            print_file(0, f);
        else
        {
            int fd = open_file(argv[i]);
            if (fd == -1)
            {
                exit_code = 1;
                continue;
            }
            print_file(fd, f);
        }
    }
    
    return exit_code;
}