#ifndef FLAG_H
#define FLAG_H

// Constructors for flags
#define arg_flag(cname, fname, fdesc) arg_flag_t* cname = set_arg_flag(fname, fdesc);
#define bool_flag(cname, fname, fdesc) bool_flag_t* cname = set_bool_flag(fname, fdesc);
#define pos_flag(cname, fname, fdesc, pos) pos_flag_t* cname = set_pos_flag(pos, fname, fdesc);

// removes valid flags from argc, argv --- stores them in global FLAG_BUFFER
static void filter_flags(int* argc, char** argv);
#endif

#ifndef FLAG_H_IMPLEMENTATION
#define FLAG_H_IMPLEMENTATION
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    BOOL,
    ARG,
    POS,
} flag_type_t;

typedef struct {
    bool valid;
    char* content;
} arg_flag_t;

typedef struct {
    bool valid;
} bool_flag_t;

typedef struct {
    bool valid;
    size_t pos;
} pos_flag_t;

typedef struct {
    flag_type_t type;
    const char* name;
    const char* description;
    union {
        pos_flag_t pos_flag;
        arg_flag_t arg_flag;
        bool_flag_t bool_flag;
    };
} flag_t;

#ifndef FLAG_CAPACITY
#define FLAG_CAPACITY 1
#endif

#define ASSERT(condition, ...)        \
    if (!(condition)) {               \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    }

// define FLAG_CAPACITY above #include "flag.h"
static flag_t FLAG_BUFFER[FLAG_CAPACITY] = { { .name = NULL } };
static size_t FLAG_COUNT = 0;

inline static size_t hash(const char* s)
{
    size_t index = 0;
    while (*s != '\0')
        index *= *s++;

    return index % FLAG_CAPACITY;
}

// for -h and --help
inline static void dump_descriptions()
{
    // to check if name and description exist
    const char* description = NULL;

    for (int i = 0; i < FLAG_CAPACITY; i++) {
        description = FLAG_BUFFER[i].description;

        // printf with description check
        printf("%s\t%s\n", FLAG_BUFFER[i].name, (description) ? description : "");
    }
    printf("\n");
}

inline static flag_t* set_general_flag(const char* name, const char* description, flag_type_t type)
{
    ASSERT(++FLAG_COUNT <= FLAG_CAPACITY, "ERROR: Too many Flags -- define FLAG_CAPACITY to resolve this\n");
    size_t index = hash(name);
    // compensate collision
    while (FLAG_BUFFER[index].name != NULL)
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0; // flip from arr len to 0

    FLAG_BUFFER[index].name = name;
    FLAG_BUFFER[index].type = type;
    FLAG_BUFFER[index].description = description;

    return &FLAG_BUFFER[index];
}

inline static arg_flag_t* set_arg_flag(const char* name, const char* description)
{
    flag_t* flag = set_general_flag(name, description, ARG);
    flag->arg_flag.valid = false;
    flag->arg_flag.content = NULL;
    return &flag->arg_flag;
}

inline static bool_flag_t* set_bool_flag(const char* name, const char* description)
{
    flag_t* flag = set_general_flag(name, description, BOOL);
    flag->bool_flag.valid = false;
    return &flag->bool_flag;
}

inline static pos_flag_t* set_pos_flag(size_t pos, const char* name, const char* description)
{
    flag_t* flag = set_general_flag(name, description, POS);
    flag->pos_flag.valid = false;
    flag->pos_flag.pos = pos;
    return &flag->pos_flag;
}

inline static flag_t* get_flag(const char* name)
{
    // index where it should be
    size_t index = hash(name);

    // compensate collision
    for (int k = 0; k < FLAG_CAPACITY; ++k) {
        if (strcmp(FLAG_BUFFER[index].name, name) == 0)
            return &FLAG_BUFFER[index];
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0; // flip from arr len to 0
    }
    return NULL;
}

inline static bool is_help_flag(char* arg)
{
    return ((strcmp(arg, "-h") & strcmp(arg, "--help")) == 0);
}

// removes valid flags from argv --- stores them in global FLAG_BUFFER
// no clue why this is static -- but resolves warnings
static void filter_flags(int* argc, char** argv)
{
    flag_t* flag = NULL;
    int rest_counter = 0;

    for (int i = 0; i < *argc; i++) {

        flag = get_flag(argv[i]);

        if (flag == NULL) {
            if (is_help_flag(argv[i]))
                dump_descriptions();
            else
                argv[rest_counter++] = argv[i];
        } else
            switch (flag->type) {
            case ARG:
                ASSERT(i + 1 < *argc && get_flag(argv[i + 1]) == NULL, "ERROR: Flag %s needs an argument\n", flag->name);
                flag->arg_flag.valid = true;
                flag->arg_flag.content = argv[++i];
                break;
            case BOOL:
                flag->bool_flag.valid = true;
                break;
            case POS:
                ASSERT(i == flag->pos_flag.pos, "ERROR: positional FLag \"%s\" needs to be in position %ld rather than %d\n", flag->name, flag->pos_flag.pos, i);
                flag->pos_flag.valid = true;
                break;
            }
    }
    argv[rest_counter] = NULL;
    *argc = rest_counter;
}
#endif
