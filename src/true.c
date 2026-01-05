/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stddef.h>
#include <string.h>

/* Defines. */

#define UTIL_NAME "true"

/* Help option data. */

static const opt_group
true_options[] = {
    {"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
    {NULL, NULL, NULL}
};

static const char*
true_option_details[] = {
    NULL
};

static const char*
true_notes[] = {
    NULL
};

/* Helpers. */

static void
help_option()
{
    help_info info = {
        "true",
        "Exit with status code 0 (always).",
        true_options,
        true_option_details,
        true_notes
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
    return 0;
}