#include "../include/utils.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define GETCWD _getcwd
#else
    #include <unistd.h>
    #define GETCWD getcwd
#endif

NORETURN void
end(const char* util_name, const char* src, const char* msg)
{
    if (util_name != NULL)
        fprintf(stderr, "%s - ", util_name);
    fprintf(stderr, "Error (%s): %s\n", src, msg);
    exit(EXIT_FAILURE);
}

void
error(const char* util_name, int exit_code, const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    fprintf(stderr, "%s: ", util_name);
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    if (exit_code != -1)
        exit(exit_code);
}

char*
p_getcwd()
{
    char* path = calloc(1024, sizeof(char));
    if (path == NULL)
        end(NULL, "p_getcwd", "Allocation failure.");

    if (GETCWD(path, 1023) == NULL)
        end(NULL, "p_getcwd", "Library failure.");
    
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
    if (options[0].option != NULL) printf("Options:\n");
    for (int i = 0; options[i].option != NULL; i++)
    {
        if (options[i].alt == NULL)
            printf("%4s%-12s%s\n", "", options[i].option,
                options[i].effect);
        else
        {
            printf("%4s%s, %s\n", "", options[i].option, options[i].alt);
            printf("%16s%s\n", "", options[i].effect);
        }
        printf("\n");
    }

    if (info->option_details[0] != NULL) printf("Option details:\n");
    for (int i = 0; info->option_details[i] != NULL; i++)
        printf("%4s%s\n", "", info->option_details[i]);
    if (info->option_details[0] != NULL) printf("\n");

    if (info->notes[0] != NULL) printf("Notes:\n");
    for (int i = 0; info->notes[i] != NULL; i++)
        printf("%4s%s\n", "", info->notes[i]);
    if (info->notes[0] != NULL) printf("\n");
}

static int
in_str_array(char* str, cl_opt arr[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (arr[i].alt == NULL)
            continue;
        size_t len = strlen(str);
        char* pos = strchr(str, '=');
        if (arr[i].has_value && (pos != NULL))
        {
            arr[i].value = pos + 1; // Skip the '=' sign.
            len -= (pos - str);
        }
        if (!strncmp(arr[i].alt, str, len))
            return i;
    }

    return -1;
}

static int
in_char_array(char c, cl_opt arr[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (arr[i].main == c)
            return i;
    }

    return -1;
}

u64
set_bitflags(cl_opt opts[], char* argv[], int count, int *skip)
{
    u64 flag = 0;

    if (!strcmp(argv[1], "-"))
        return flag;

    for (int i = 1; argv[i] != NULL; i++)
    {
        if (strncmp(argv[i], "-", 1) != 0)
            break;

        int pos = in_str_array(argv[i], opts, count);
        if (pos != -1)
        {
            opts[pos].set ? SET_BIT(flag, opts[pos].flag)
                : RESET_BIT(flag, opts[pos].flag);
            if (skip != NULL) (*skip)++;
            continue;
        }

        u64 orig_flag = flag;
        size_t len = strlen(argv[i]);
        size_t j;
        for (j = 1; j < len; j++)
        {
            int pos = in_char_array(argv[i][j], opts, count);
            if (pos != -1)
            {
                opts[pos].set ? SET_BIT(flag, opts[pos].flag)
                    : RESET_BIT(flag, opts[pos].flag);
            }
            else
                break;
        }
        if (j != len)
        {
            // The presence of any foreign letter in the option
            // combination should undo any changes to the flag
            // and terminate option parsing.
            flag = orig_flag;
            break;
        }
        if (skip != NULL) (*skip)++;
    }

    return flag;
}