#include "../include/utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    const opt_group* const options = info->options;
    if (options[0].option != NULL)
        printf("Options:\n");
    for (int i = 0; options[i].option != NULL; i++)
    {
        if (options[i].alt == NULL)
            printf("    %-16s%s\n", options[i].option,
                options[i].effect);
        else
        {

        }
    }

    for (int i = 0; info->option_details[i] != NULL; i++)
        printf("%s\n", info->option_details[i]);

    for (int i = 0; info->notes[i] != NULL; i++)
        printf("%s\n", info->notes[i]);
}

static int
in_array(char c, char arr[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (arr[i] == c)
            return i;
    }

    return -1;
}

int
set_bitflags(char opts[], char* argv[], int flags[],
    int count, int *skip)
{
    int flag = 0;

    for (int i = 1; argv[i] != NULL; i++)
    {
        if (strncmp(argv[i], "-", 1) != 0)
            break;
        size_t len = strlen(argv[i]);
        size_t j;
        for (j = 1; j < len; j++)
        {
            int pos = in_array(argv[i][j], opts, count);
            if (pos != -1)
                SET_BIT(flag, flags[pos]);
            else
                break;
        }
        if (j != len)
            break;
        (*skip)++;
    }

    return flag;
}