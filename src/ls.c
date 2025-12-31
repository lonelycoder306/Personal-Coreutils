#include "../include/utils.h"
#include <stdint.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/* 60 options for ls. */

typedef uint8_t flag;
#define DEF_FLAG (flag) 0

// static flag
// set_flags(int argc, char* argv[])
// {
//     flag f = DEF_FLAG;


// }

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;
    
    char* path;
    if (argc == 1)
        path = p_getcwd();
    else
        path = argv[1];

    DIR* dir = opendir(path);
    if (dir == NULL)
        end("opendir", "Failed to open directory.");
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if ((entry->d_name[0] == '\0')/* || (entry->d_name[0] == '.')*/)
            continue;
        printf("%s  ", entry->d_name);
    }
    closedir(dir);
    printf("\n");
    
    return 0;
}