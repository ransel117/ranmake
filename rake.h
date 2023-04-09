/*
* MIT License
*
* Copyright (c) 2023 Ransel117
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef RAKE_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define RAKE_VER "0.0.1"

#define RAKE_VALID_ARGS_AMOUNT 6
#define RAKE_VER_ARG1  "-v"
#define RAKE_VER_ARG2  "--version"
#define RAKE_HELP_ARG1 "-h"
#define RAKE_HELP_ARG2 "--help"
#define RAKE_F_ARG1 "-f"
#define RAKE_F_ARG2 "--file"
#define RAKE_DEF_F1 "rake"
#define RAKE_DEF_F2 "__rake"

#define RAKE_READ_CHUNK_SIZE 2097152
#define RAKE_READ_ERR_G "Error reading file: %s. errno: %d"
#define RAKE_READ_ERR_MEM "Not enough free memory to read file: %s"

typedef enum {
    RAKE_SUCCESS,
    RAKE_ERR,
    RAKE_ERR_F_N_FND,
} rake_error;

typedef struct {
    char *data;
    size_t len;
    bool is_valid;
} File;

#define RAKE_ERR_RET(R, ...) do { \
    fprintf(stderr, __VA_ARGS__);      \
    fprintf(stderr, "\n");             \
    return R;                          \
} while(0);
#define RAKE_ERR_EXIT(...) do { \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n");        \
    exit(1);                      \
} while(0);


int32_t rake_print_ver(void);
int32_t rake_print_help(void);
bool rake_str_eq(const char*, const char*);
bool rake_file_exists(const char*);
void rake_print_args(const char**, const uint32_t);
File *rake_read_file(const char*);
int32_t rake_main(const char*);

#endif /* RAKE_H */

#ifdef RAKE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

int32_t rake_print_ver(void) {
    return printf("Rake "RAKE_VER"\n");
}

int32_t rake_print_help(void) {
    return printf("help message\n");
}

bool rake_str_eq(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

bool rake_file_exists(const char *file) {
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

File *rake_read_file(const char *path) {
    char *data, *tmp;
    size_t used, size, n;
    File *file;
    FILE *fp;

    data = NULL;
    used = 0;
    size = 0;

    file = (File*)malloc(sizeof(File));
    if (!file) {
        RAKE_ERR_RET(file, "Unable to allocate memory");
    }
    file->is_valid = false;

    fp = fopen(path, "r");
    if (!fp || ferror(fp)) {
        RAKE_ERR_RET(file, RAKE_READ_ERR_G, path, errno);
    }

    while (true) {
        if (used + RAKE_READ_CHUNK_SIZE + 1 > size) {
            size = used + RAKE_READ_CHUNK_SIZE + 1;
            if (size <= used) {
                free(data);
                RAKE_ERR_RET(file, "Input file too large: %s", path);
            }

            tmp = realloc(data, size);
            if (!tmp) {
                free(data);
                RAKE_ERR_RET(file, RAKE_READ_ERR_MEM, path);
            }
            data = tmp;
        }

        n = fread(data + used, 1, RAKE_READ_CHUNK_SIZE, fp);
        if (n == 0) {
            break;
        }

        used += n;
    }

    if (ferror(fp)) {
        free(data);
        RAKE_ERR_RET(file, RAKE_READ_ERR_G, path, errno);
    }

    tmp = realloc(data, used + 1);
    if (!tmp) {
        free(data);
        RAKE_ERR_RET(file, RAKE_READ_ERR_MEM, path);
    }

    data = tmp;
    data[used] = 0;

    file->data = data;
    file->len = used;
    file->is_valid = true;

    return file;
}

char *get_value(const char *config_buffer, const char *value) {
    char *line;

    line = strstr(config_buffer, value);
    if (!line) {
        RAKE_ERR_EXIT("Could not find value: %s.\n", value);
    }

    size_t len;
    char *end, *curr, *tmp_ptr, *tmp_buffer;
    tmp_buffer = (char*)malloc(sizeof(char*)*20);

    len = strlen(line);
    end = line + len;
    curr = line;
    tmp_ptr = &tmp_buffer[0];

    // Move pointer to '='.
    while (*curr != '=' && curr != end) ++curr;
    // Consume '='.
    ++curr;
    // Consume any spaces.
    while (*curr == ' ') ++curr;
    // Get characters until end of line.
    while (*curr != '\n' && *curr != 0 && curr != end) *tmp_ptr++ = *curr++;

    *(tmp_ptr+1) = 0;

    return tmp_buffer;
}

void dostuff(const char *path) {
    File *file;
    char *compiler, *flags, *defines, *includes, *files, *libs, *exec, *output, *cmd;

    file = rake_read_file(path);

    compiler = get_value(file->data, "CC");
    flags    = get_value(file->data, "FLAGS");
    defines  = get_value(file->data, "DEFINES");
    includes = get_value(file->data, "INCLUDES");
    files    = get_value(file->data, "FILES");
    libs     = get_value(file->data, "LIBS");
    exec     = get_value(file->data, "EXEC");
    output = rake_str_eq(compiler, "cl") || rake_str_eq(compiler, "CL") || rake_str_eq(compiler, "clang-cl") ? "/OUT:" : "-o";
    cmd = (char*)malloc(sizeof(char*)*8);

    snprintf(cmd, sizeof(cmd)*8, "%s %s %s %s %s %s %s %s", compiler, flags, defines, includes, files, libs, output, exec);


    printf("%s\n", cmd);
    system(cmd);
}

int32_t rake_main(const char *path) {
    if (rake_str_eq(path, "")) {
        if (rake_file_exists(RAKE_DEF_F1)) {
            dostuff(RAKE_DEF_F1);
        } else if (rake_file_exists(RAKE_DEF_F2)) {
            dostuff(RAKE_DEF_F2);
        } else {
            RAKE_ERR_RET(RAKE_ERR_F_N_FND, "Could not find the default file: "RAKE_DEF_F1" or "RAKE_DEF_F2);
        }
    } else {
        if (!rake_file_exists(path)) {
            RAKE_ERR_RET(RAKE_ERR_F_N_FND, "Could not find the file: %s", path);
        }
        dostuff(path);
    }

    return RAKE_SUCCESS;
}

#endif /* RAKE_IMPLEMENTATION */
