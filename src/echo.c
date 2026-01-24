/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "echo"

typedef u8 flag;
#define DEF_FLAG (flag) 0x0

#define OCT_LIMIT   3
#define HEX_LIMIT   2

#define LETTER_OPTS 3 // -n, -e, -E

/* 5 options for echo. */

/* Option bits. */

#define HELP_BIT        (1 << 7)
#define VERSION_BIT     (1 << 6)
#define NL_BIT          (1 << 5)
#define INTER_BIT       (1 << 4)
#define NO_INTER_BIT    (1 << 3)

/* Option bit-masks. */

#define HELP(f)     ((f) & HELP_BIT)
#define VERSION(f)  ((f) & VERSION_BIT)
#define NO_NL(f)    ((f) & NL_BIT)
#define INTER(f)    ((f) & INTER_BIT)
#define NO_INTER(f) ((f) & NO_INTER_BIT)

/* Help option data. */

static const opt_group
echo_options[] = {
    {"-n", NULL, "Do not output the trailing newline."},
    {"-e", NULL, "Enable interpretation of backslash escapes."},
    {"-E", NULL, "Disable interpretation of backslash escape (default)."},
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
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
            {'n', NULL, NL_BIT, SET},
            {'e', NULL, INTER_BIT, SET},
            {'E', NULL, NO_INTER_BIT, SET}
        };
        f = (flag) set_bitflags(opts, argv, LETTER_OPTS, skip);
    }

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

static inline bool
is_oct_char(char c)
{
    return (isdigit(c) && (c >= '0') && (c <= '7'));
}

static inline bool
is_hex_char(char c)
{
    if (isdigit(c)) return true;
    c = tolower(c);
    return ((c >= 'a') && (c <= 'f'));
}

static char
from_oct(const char* str, size_t* idx)
{
    char c = 0;
    int i = 0;
    while ((i < OCT_LIMIT) && is_oct_char(str[i]))
    {
        c = c * 8 + (str[i] - '0');
        (*idx)++;
        i++;
    }

    return c;
}

static char
from_hex(const char* str, size_t* idx)
{
    char c = 0;
    int i = 0;
    while ((i < HEX_LIMIT) && is_hex_char(str[i]))
    {
        if (isdigit(str[i]))
            c = c * 16 + (str[i] - '0');
        else if (islower(str[i]))
            c = c * 16 + (str[i] - 'a' + 10);
        else
            c = c * 16 + (str[i] - 'A' + 10);
        (*idx)++;
        i++;
    }

    return c;
}

static char*
format_str(const char* str)
{
    size_t len = strlen(str);
    char* fmt = calloc(len + 1, sizeof(char));
    if (fmt == NULL)
        end(UTIL_NAME, "format_str", "Allocation failure.");

    size_t i = 0;
    for (size_t j = 0; str[j] != '\0';)
    {
        if ((str[j] == '\\') && (j != len - 1))
        {
            switch (str[j + 1])
            {
                case '\\':  fmt[i++] = '\\';    break;
                case 'a':   fmt[i++] = '\a';    break;
                case 'b':   fmt[i++] = '\b';    break;
                case 'e':   fmt[i++] = '\e';    break;
                case 'f':   fmt[i++] = '\f';    break;
                case 'n':   fmt[i++] = '\n';    break;
                case 'r':   fmt[i++] = '\r';    break;
                case 't':   fmt[i++] = '\t';    break;
                case 'v':   fmt[i++] = '\v';    break;
                case '0':
                {
                    char temp = from_oct(&str[j + 2], &j);
                    if (temp != '\0')
                        fmt[i++] = temp;
                    break;
                }
                case 'x':
                {
                    char temp = from_hex(&str[j + 2], &j);
                    if (temp != '\0')
                        fmt[i++] = temp;
                    break;
                }
                case 'c':
                {
                    printf("%s", fmt);
                    exit(EXIT_SUCCESS);
                }
                default:
                {
                    if (isdigit(str[j + 1]))
                    {
                        char temp = from_oct(&str[j + 1], &j);
                        if (temp != '\0')
                            fmt[i++] = temp;
                        j -= 1; // To not skip two below.
                    }
                    else
                    {
                        fmt[i++] = '\\';
                        j -= 1; // To not skip below.
                    }
                    break;
                }
            }
            j += 2;
        }
        else
            fmt[i++] = str[j++];
    }

    return fmt;
}

/* Main. */

int
main(int argc, char* argv[])
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
    
    for (int i = skip; argv[i] != NULL; i++)
    {
        char* arg = argv[i];
        if (INTER(f) && !NO_INTER(f))
        {
            char* temp = format_str(arg);
            printf("%s", temp);
            free(temp);
        }
        else
            printf("%s", arg);
        if (argv[i + 1] != NULL) printf(" ");
    }

    if (!NO_NL(f))
        printf("\n");
    
    return 0;
}