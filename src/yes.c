/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "yes"

/* Help option data. */

static const opt_group
yes_options[] = {
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
};

static const char*
yes_option_details[] = {
    NULL
};

static const char*
yes_notes[] = {
    NULL
};

/* Helpers. */

static void
help_option()
{
    help_info info = {
        "yes [STRING]...\n" \
        "   Or:   yes [OPTION]",
        "Repeatedly output a line with all specified STRING(s), or 'y'.",
        yes_options,
        yes_option_details,
        yes_notes
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
    
    if (argc == 1)
    {
        while (true)
            printf("y\n");
    }
    else
    {
        while (true)
        {
            for (int i = 1; argv[i] != NULL; i++)
                printf("%s ", argv[i]);
            printf("\n");
        }
    }

    return 0;
}