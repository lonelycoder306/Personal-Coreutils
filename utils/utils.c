#include "../include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
end(char* src, char* msg)
{
    fprintf(stderr, "Error (%s): %s\n", src, msg);
    exit(EXIT_FAILURE);
}

char*
p_getcwd()
{
    char* path = calloc(1024, sizeof(char));
    if (path == NULL)
        end("p_getcwd", "Allocation failure.");

    if (getcwd(path, 1023) == NULL)
        end("p_getcwd", "Library failure.");
    
    return path;
}

void
display_version(const char* util_name)
{
    printf("%s (personal coreutils) %s\n",
        util_name, VERSION_STR);
}

void
display_help(help_info* info)
{
    printf("Usage:   %s\n", info->usage);
    printf("Purpose: %s\n\n", info->purpose);

    const opt_pair* const options = info->options;
    if (options[0].option != NULL)
        printf("Options:\n");
    for (int i = 0; options[i].option != NULL; i++)
    {
        printf("    %-16s%s\n", options[i].option,
            options[i].effect);
    }

    for (int i = 0; info->option_details[i] != NULL; i++)
        printf("%s\n", info->option_details[i]);

    for (int i = 0; info->notes[i] != NULL; i++)
        printf("%s\n", info->notes[i]);
}