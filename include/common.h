#ifndef COMMON_H
#define COMMON_H

/* 
*  Holds any constant macros or data types/structures
*  that we use throughout the project.
*/

#define VERSION_STR "0.0.1"

#include <stdint.h>
#include <stdbool.h>

#define SET     true
#define RESET   false

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef struct cl_opt {
    char        main;
    const char* alt;
    u64         flag;
    bool        set;
} cl_opt; // Command-line option struct.

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