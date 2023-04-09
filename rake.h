#ifndef RAKE_H
#include <stdint.h>
#include <stdbool.h>

#define RAKE_VER "0.0.1"

#define RAKE_VALID_ARGS_AMOUNT 6
#define RAKE_VER_ARG1  "-v"
#define RAKE_VER_ARG2  "--version"
#define RAKE_HELP_ARG1 "-h"
#define RAKE_HELP_ARG2 "--help"
#define RAKE_FILE_ARG1 "-f"
#define RAKE_FILE_ARG2 "--file"
#define RAKE_DEFAULT_FILE1 "rake"
#define RAKE_DEFAULT_FILE2 "__rake"

enum rake_error {
    RAKE_SUCCESS,
    RAKE_ERROR,
    RAKE_ERROR_FILE_NOT_FOUND,
};

int32_t rake_print_ver(void);
int32_t rake_print_help(void);
bool rake_str_eq(const char*, const char*);
bool rake_check_file_exists(const char*);
void rake_print_args(const char**, const uint32_t);
int32_t rake_main(const char*);

#endif /* RAKE_H */

#ifdef RAKE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>

int32_t rake_print_ver(void) {
    return printf("Rake "RAKE_VER"\n");
}

int32_t rake_print_help(void) {
    return printf("help message\n");
}

bool rake_str_eq(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

bool rake_check_file_exists(const char *file) {
    FILE *f;

    f = fopen(file, "r");
    if (f) {
        fclose(f);
        return true;
    }

    return false;
}

void rake_print_args(const char **args, const uint32_t count) {
    uint32_t i;

    for (i = 1; i < count; ++i) {
        printf("%s\n", args[i]);
    }
}

void dostuff(const char *file) {
    printf("%s\n", file);
}

int32_t rake_main(const char *file) {
    if (rake_str_eq(file, "")) {
        if (rake_check_file_exists(RAKE_DEFAULT_FILE1)) {
            dostuff(RAKE_DEFAULT_FILE1);
        } else if (rake_check_file_exists(RAKE_DEFAULT_FILE2)) {
            dostuff(RAKE_DEFAULT_FILE2);
        } else {
            printf("Could not find the default file: "RAKE_DEFAULT_FILE1" or "RAKE_DEFAULT_FILE2"\n");
            return RAKE_ERROR_FILE_NOT_FOUND;
        }
    } else {
        if (rake_check_file_exists(file)) {
            dostuff(file);
        } else {
            printf("Could not find the file: %s\n", file);
            return RAKE_ERROR_FILE_NOT_FOUND;
        }
    }

    return RAKE_SUCCESS;
}

#endif /* RAKE_IMPLEMENTATION */
