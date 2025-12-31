#ifndef COMMON_H
#define COMMON_H

/* 
*  Holds any constant macros or data types/structures
*  that we use throughout the project.
*/

#define VERSION_STR "0.0.1"

typedef struct opt_group {
    const char* option;
    const char* alt;
    const char* effect;
} opt_group;

typedef struct help_info {
    const char*             usage;
    const char*             purpose;
    const opt_group* const  options;
    const char* const*      option_details;
    const char* const*      notes;
} help_info;

#endif