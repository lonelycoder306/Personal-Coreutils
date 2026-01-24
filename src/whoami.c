/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #include <pwd.h>
    #include <unistd.h>
#endif

/* Defines. */

#define UTIL_NAME "whoami"

/* Help option data. */

static const opt_group
whoami_options[] = {
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
};

static const char*
whoami_option_details[] = {
    NULL
};

static const char*
whoami_notes[] = {
    NULL
};

/* Helpers. */

static void
help_option()
{
    help_info info = {
        "whoami [OPTION]...",
        "Print the user name associated with the current effective user ID.",
        whoami_options,
        whoami_option_details,
        whoami_notes,
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
        if ((ret == 0) || (ret > 256))
            end(UTIL_NAME, "GetUserName", "Library failure.");
        printf("%s\n", name);
    #else
        uid_t id = geteuid();
        struct passwd* pw = getpwuid(id);
        if (pw == NULL)
            end(UTIL_NAME, "getpwuid", "Library failure.");
        printf("%s\n", pw->pw_name);
    #endif

    return 0;
}