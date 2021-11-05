#ifndef FLAG_H
#define FLAG_H

//Constructors for flags
#define arg_flag(cname, fname, fdesc) flag_t* cname = set_flag(1, fname, fdesc);
#define bool_flag(cname, fname, fdesc) flag_t* cname = set_flag(0, fname, fdesc);

//removes valid flags from argc, argv --- stores them in global FLAG_BUFFER
void filter_flags(int* argc, char** argv);
#endif

#ifndef FLAG_H_IMPLEMENTATION
#define FLAG_H_IMPLEMENTATION
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Multiple Argument Flag
// TODO: Positionale Arguments

typedef struct {
    bool has_arg;
    bool valid;
    char* content;
    const char* name;
    const char* description;
} flag_t;

#ifndef FLAG_CAPACITY
#define FLAG_CAPACITY -1 //little Hack (?)better Way(?)
#endif

#define ASSERT(condition, ...)        \
    if (!(condition)) {               \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    }

//define FLAG_CAPACITY above #include "flag.h"
flag_t FLAG_BUFFER[FLAG_CAPACITY] = { { .name = NULL, .valid = false, .content = NULL } };

// TODO: Implement Perfect Hashing
size_t hash(const char* s)
{
    size_t index = 0;
    while (*s != '\0')
        index *= *s++;

    return index % FLAG_CAPACITY;
}

//for -h and --help
void dump_descriptions()
{
    //to check if name and description exist
    const char* description = NULL;

    for (int i = 0; i < FLAG_CAPACITY; i++) {
        description = FLAG_BUFFER[i].description;

        //printf with description check
        printf("%s\t%s\n", FLAG_BUFFER[i].name, (description) ? description : "");
    }
    printf("\n");
}

// TODO: Do this at compile time
//finds right slot in global FLAG array, sets Flag and returns pointer to that slot
flag_t* set_flag(const bool has_arg, const char* name, const char* description)
{
    //name must exist
    ASSERT(name != NULL, "Flags need a name\n");

    //index where it should be
    size_t index = hash(name);

    //compensate collision
    while (FLAG_BUFFER[index].name != NULL)
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0; //flip from arr len to 0

    FLAG_BUFFER[index].name = name;
    FLAG_BUFFER[index].has_arg = has_arg;
    FLAG_BUFFER[index].description = description;

    return &FLAG_BUFFER[index];
}

flag_t* get_flag(const char* name)
{
    //index where it should be
    size_t index = hash(name);

    //compensate collision
    for (int k = 0; k < FLAG_CAPACITY; ++k) {
        if (strcmp(FLAG_BUFFER[index].name, name) == 0)
            return &FLAG_BUFFER[index];
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0; //flip from arr len to 0
    }
    return NULL;
}

bool is_help_flag(char* arg)
{
    return ((strcmp(arg, "-h") & strcmp(arg, "--help")) == 0);
}

//removes valid flags from argv --- stores them in global FLAG_BUFFER
void filter_flags(int* argc, char** argv)
{
    flag_t* flag = NULL;

    //new argc
    int rest_counter = 0;

    for (int i = 0; i < *argc; ++i) {
        //check if flag exists
        flag = get_flag(argv[i]);
        if (flag != NULL) {

            flag->valid = true;

            if (flag->has_arg) {
                //Next argv must be an argument a flag
                ASSERT(i + 1 < *argc && get_flag(argv[i + 1]) == NULL, "ERROR: %s needs an argument\n", flag->name);

                flag->content = argv[++i];
            }
        } else {

            if (is_help_flag(argv[i])) //-h and --help aren't stored in FLAG_BUFFER
                dump_descriptions();
            else
                //flag does not exist -> store argv[i] in argv
                argv[rest_counter++] = argv[i];
        }
    }

    //argv has to be NULL terminated
    argv[rest_counter] = NULL;
    *argc = rest_counter;
}
#endif
