/* Includes. */

#include "../include/common.h"
#include "../include/utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
	#include <fileapi.h>
	#include <windows.h>
#endif

/* Defines. */

#define UTIL_NAME "pwd"
typedef u8 flag;
#define DEF_FLAG (flag) 0x0

#define LETTER_OPTS 2 // -L, -P

/* 4 options for pwd. */

/* Option bits. */

#define HELP_BIT        (1 << 7)
#define VERSION_BIT     (1 << 6)
#define LOGICAL_BIT		(1 << 5)
#define PHYSICAL_BIT	(0x0)

/* Option bit-masks. */

#define HELP(f)     ((f) & HELP_BIT)
#define VERSION(f)  ((f) & VERSION_BIT)
#define LOGICAL(f)	((f) & LOGICAL_BIT)

/* Help option data. */

static const opt_group
pwd_options[] = {
	{"-L", "--logical", "Use PWD from environment, " \
		"even if it contains symlinks."},
	{"-P", "--physical", "Resolve all symlinks in path."},
	{"--help", NULL, "Display this help page and exit."},
    {"--version", NULL, "Output version information and exit."},
	{NULL, NULL, NULL}
};

static const char*
pwd_option_details[] = {
	NULL
};

static const char*
pwd_notes[] = {
	"If no option is specified, -P is assumed.",
	NULL
};

/* Helpers. */

static flag
set_flags(int argc, char* argv[], int* skip)
{
    flag f = DEF_FLAG;
    
    if (argc < 2) return f;

    if (!strcmp(argv[1], "--help") && (argc == 2))
        SET_BIT(f, HELP_BIT);
    else if (!strcmp(argv[1], "--version") && (argc == 2))
        SET_BIT(f, VERSION_BIT);
    else
    {
        cl_opt opts[LETTER_OPTS]  = {
			{'L', "--logical", LOGICAL_BIT},
			{'P', "--physical", PHYSICAL_BIT}
        };
        f = (flag) set_bitflags(opts, argv, LETTER_OPTS, skip);
    }

    return f;
}

static void
help_option()
{
    help_info info = {
        "pwd [OPTION]...",
        "Print the full filename of the current working directory.",
        pwd_options,
        pwd_option_details,
        pwd_notes,
    };

    display_help(&info);
}

// Do NOT free return value.
static char*
get_current_dir()
{
	#if defined(_WIN32) || defined(_WIN64)
		static char path[256];
		DWORD ret = GetCurrentDirectory(256, path);
		if ((ret == 0) || (ret > 256))
			end(UTIL_NAME, "GetCurrentDirectory", "Library failure.");
		return path;
	#else
		static char* path;
		path = getenv("PWD");
		if (path == NULL)
			end(UTIL_NAME, "getenv", "Library failure.");
		return path;
	#endif
}

// Free return value.
static char*
resolve_path(char* path)
{
	#if defined(_WIN32) || defined(WIN64)
		char* real = malloc(256 * sizeof(char));
		if (real == NULL)
			end(UTIL_NAME, "calloc", "Allocation failure.");
		DWORD ret = GetFullPathName(path, 256, real, NULL);
		if ((ret == 0) || (ret > 256))
			end(UTIL_NAME, "GetFullPathName", "Library failure.");
		return real;
	#else
		char* real = realpath(path, NULL);
		if (real == NULL)
			end(UTIL_NAME, "realpath", "Library failure.");
		return real;
	#endif
}

/* Main. */

int
main(int argc, char* argv[])
{
	flag f = set_flags(argc, argv, NULL);

	if (HELP(f))
	{
		help_option();
        exit(EXIT_SUCCESS);
	}
	else if (VERSION(f))
	{
		display_version(UTIL_NAME);
		exit(EXIT_SUCCESS);
	}

	char* path = get_current_dir();
	if (LOGICAL(f))
		printf("%s\n", path);
	else
	{
		char* real = resolve_path(path);
		printf("%s\n", real);
		free(real);
	}

	return 0;
}