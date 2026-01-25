/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

/* Defines. */

#define UTIL_NAME "logname"

/* Help option data. */

static const opt_group
logname_options[] = {
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
};

static const char*
logname_option_details[] = {
    NULL
};

static const char*
logname_notes[] = {
    NULL
};

/* Helpers. */

static void
help_option()
{
    help_info info = {
        "logname [OPTION]...",
        "Print the user's login name.",
        logname_options,
        logname_option_details,
        logname_notes,
    };

    display_help(&info);
}

/* Main. */

int
main(int argc, char* argv[])
{
    if ((argc == 2) && !strcmp(argv[1], "--help"))
    {
        help_option();
        exit(EXIT_SUCCESS);
    }
    else if ((argc == 2) && !strcmp(argv[1], "--version"))
    {
        display_version(UTIL_NAME);
        exit(EXIT_SUCCESS);
    }

    #if defined(_WIN32) || defined(_WIN64)
        char name[256];
        unsigned long name_len;
        BOOL ret = GetUserName(name, &name_len);
        if (ret > 256)
            end(UTIL_NAME, "GetUserName", "Library failure.");
        else if (ret == 0)
            error(UTIL_NAME, 1, "No login name.");
    #else
        char* name = getlogin();
        if (name == NULL)
            error(UTIL_NAME, 1, "No login name.");
    #endif

    printf("%s\n", name);
    return 0;
}