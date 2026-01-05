/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stddef.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "false"

/* Help option data. */

static const opt_group
false_options[] = {
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
};

static const char*
false_option_details[] = {
    NULL
};

static const char*
false_notes[] = {
    NULL
};

/* Helpers. */

static void
help_option()
{
    help_info info = {
        "false",
        "Exit with status code 1 (always).",
        false_options,
        false_option_details,
        false_notes
    };

    display_help(&info);
}

/* Main. */

int
main(int argc, char *argv[])
{
    if ((argc == 2) && !strcmp(argv[1], "--help"))
        help_option();
    else if ((argc == 2) && !strcmp(argv[1], "--version"))
        display_version(UTIL_NAME);
    return 1;
}