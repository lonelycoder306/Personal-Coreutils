/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "dirname"

typedef u8 flag;
#define DEF_FLAG (flag) 0x0

#define LETTER_OPTS 1 // -z

#if defined(_WIN32) || defined(_WIN64)
    #define PATH_SEP '\\'
#else
    #define PATH_SEP '/'
#endif

/* 5 options for basename. */

/* Option bits. */

#define HELP_BIT        (1 << 7)
#define VERSION_BIT     (1 << 6)
#define ZERO_BIT        (1 << 5)

/* Option bit-masks. */

#define HELP(f)     ((f) & HELP_BIT)
#define VERSION(f)  ((f) & VERSION_BIT)
#define ZERO(f)     ((f) & ZERO_BIT)

/* Help option data. */

static const opt_group
dirname_options[] = {
    {"-z", "--zero", "No trailing newline after each name."},
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
};

static const char*
dirname_option_details[] = {
    NULL
};

static const char*
dirname_notes[] = {
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
            {'z', "--zero", ZERO_BIT, SET}
        };
        f = (flag) set_bitflags(opts, argv, LETTER_OPTS, skip);
    }

    return f;
}

static void
help_option()
{
    help_info info = {
        "dirname [OPTION] [NAME]...",
        "Output each NAME with its last non-slash component and trailing slashes removed;" \
        "\n         if NAME contains no /'s, output '.' (meaning the current directory).",
        dirname_options,
        dirname_option_details,
        dirname_notes,
    };

    display_help(&info);
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
get_dirname(const char* path)
{
    char* pos = strchr(path, PATH_SEP);
    if (pos == NULL) // No path separator.
        return strdup(".");
    else if (pos == path) // Path separator in the beginning.
    {
        pos = strchr(path + 1, PATH_SEP); // Skip the beginning.
        if (pos == NULL) return strdup(path); // No other separator character.
        return substr(path, pos - path);
    }
    else
        return substr(path, pos + 1 - path); // Include separator on the right.
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

    for (int i = skip; i < argc; i++)
    {
        char* print = get_dirname(argv[i]);
        printf("%s%s", print, ZERO(f) ? "" : "\n");
        free(print);
    }

    return 0;
}