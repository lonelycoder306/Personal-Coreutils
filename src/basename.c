/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "basename"

typedef u8 flag;
#define DEF_FLAG (flag) 0x0

#define LETTER_OPTS 3 // -a, -s, -z
#define VALUE_OPTS  1 // -s/--suffix

#if defined(_WIN32) || defined(_WIN64)
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif

#define SUFFIX_POS  0

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

static const opt_group
basename_options[] = {
    {"-a", "--multiple", "Support multiple arguments and treat each as NAME."},
    {"-s", "--suffix=SUFFIX", "Remove a trailing suffix; implies '-a'."},
    {"-z", "--zero", "No trailing newline after each name."},
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
};

static const char*
basename_option_details[] = {
    "Mandatory arguments to long options are mandatory for short options too.",
    NULL
};

static const char*
basename_notes[] = {
    NULL
};

/* Helpers. */

static flag
set_flags(int argc, char* argv[], int* skip, const char* values[])
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
            {'a', "--multiple", MULTIPLE_BIT, SET, false, NULL},
            {'s', "--suffix", SUFFIX_BIT, SET, true, NULL},
            {'z', "--zero", ZERO_BIT, SET, false, NULL}
        };
        f = (flag) set_bitflags(opts, argv, LETTER_OPTS, skip);
        int pos = 0;
        for (int i = 0; i < LETTER_OPTS; i++)
        {
            if (opts[i].has_value && (opts[i].value != NULL))
                values[pos++] = opts[i].value;
        }
    }

    return f;
}

static void
help_option()
{
    help_info info = {
        "basename NAME [SUFFIX]\n         basename OPTION... NAME...",
        "Print NAME with any leading directory components removed.\n" \
        "         If specified, also remove a trailing SUFFIX.",
        basename_options,
        basename_option_details,
        basename_notes,
    };

    display_help(&info);
}

static char*
reverse_str(char* str, size_t len)
{
    size_t max = len / 2;
    for (size_t i = 0; i < max; i++)
    {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }

    return str;
}

static char*
copy_name(char* ret, const char* path, size_t len)
{
    // Store path string in reverse.
    size_t i = (len == 0 ? SIZE_MAX : len - 1);
    size_t pos = 0;
    if (i != SIZE_MAX)
    {
        while (path[i] != PATH_SEP)
        {
            ret[pos++] = path[i];
            if (i == 0) break;
            i--; 
        }
    }

    // Reverse string back and return.
    return reverse_str(ret, pos);
}

static char*
get_basename(const char* path)
{
    size_t len = strlen(path);
    char* ret = calloc(len + 1, sizeof(char));
    if (ret == NULL)
        end(UTIL_NAME, "get_basename", "Allocation failure.");
    if (len == 0)
        return ret;

    // Skip separator(s) at end, if any.
    while ((len != 0) && path[len - 1] == PATH_SEP)
        len--;
    
    // Entire path is just separators; return one of them.
    if (len == 0)
    {
        ret[0] = PATH_SEP;
        return ret;
    }

    return copy_name(ret, path, len);
}

/* Main. */

int
main(int argc, char* argv[])
{
    int skip = 1;
    const char* values[VALUE_OPTS + 1] = {NULL};
    flag f = set_flags(argc, argv, &skip, values);

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

    const char* suffix = "";
    if (SUFFIX(f))
    {
        if (values[SUFFIX_POS] == NULL)
            suffix = argv[skip++];
        else
            suffix = values[SUFFIX_POS];
    }

    if (skip >= argc)
        error(UTIL_NAME, 1, "Missing operand.");
    else if (!MULTIPLE(f) && (skip < argc - 2))
        error(UTIL_NAME, 1, "Extra operand '%s'.", argv[skip + 2]);

    for (int i = skip; i < argc; i++)
    {
        char* print = get_basename(argv[i]);

        // Suffix may also be specified after single-argument without -s option.
        if (!MULTIPLE(f) && (skip < argc - 1))
            suffix = argv[++i];
        char* found = strstr(print, suffix);

        // 1. Suffix not found, or
        // 2. "Suffix" is equivalent to filename, or
        // 3. No suffix specified.
        if ((found == NULL) || (found == print) || (*suffix == '\0'))
            printf("%s%s", print, ZERO(f) ? "" : "\n");
        else
            printf("%.*s%s", (int) (found - print), print, ZERO(f) ? "" : "\n");
        free(print);
        if (!MULTIPLE(f)) break;
    }

    return 0;
}