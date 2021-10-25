#ifndef FLAG_H
#define FLAG_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Comment Code

typedef enum {
    BOOL,
    STR,
} type_t;

typedef struct
{
    type_t type;
    bool valid;
    void* content;
    const char* name;
} flag_t;

flag_t* set_flag(const type_t type, const char* name);
void filter_flags(int* argc, char** argv);
#endif

#ifndef FLAG_H_IMPLEMENTATION
#define FLAG_H_IMPLEMENTATION

#ifndef FLAG_CAPACITY
#define FLAG_CAPACITY -1
#endif

//define FLAG_CAPACITY above #include "flag.h"
flag_t FLAG_BUFFER[FLAG_CAPACITY] = { { .name = NULL, .valid = false, .content = NULL } };

size_t hash(const char* s)
{
    size_t index = 0;
    while (*s != '\0')
        index += *s++;

    return index % FLAG_CAPACITY;
}

flag_t* set_flag(const type_t type, const char* name)
{
    size_t index = hash(name);
    while (FLAG_BUFFER[index].name != NULL)
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0;
    FLAG_BUFFER[index].name = name;
    FLAG_BUFFER[index].type = type;
    return &FLAG_BUFFER[index];
}

flag_t* get_flag(const char* name)
{
    size_t index = hash(name);
    for (int k = 0; k < FLAG_CAPACITY; ++k) {
        if (strcmp(FLAG_BUFFER[index].name, name) == 0)
            return &FLAG_BUFFER[index];
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0;
    }
    return NULL;
}

// TODO: Refactor that shit
void filter_flags(int* argc, char** argv)
{
    flag_t* flag = NULL;
    char* rest_buffer[*argc];
    int rest_counter = 0;
    for (int i = 0; i < *argc; ++i) {
        flag = get_flag(argv[i]);
        if (flag != NULL) {
            flag->valid = true;
            switch (flag->type) {
            case STR:
                ++i;
                assert(i < *argc || get_flag(argv[i]) == NULL);
                flag->content = argv[i];
                break;
            case BOOL:
                flag->content = (void*)&flag->valid;
                break;
            }

        } else {
            rest_buffer[rest_counter++] = argv[i];
        }
    }
    *argc = rest_counter;
    for (int i = 0; i < *argc; ++i)
        argv[i] = rest_buffer[i];
}
#endif
