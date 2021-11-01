#ifndef FLAG_H
#define FLAG_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//test comment

typedef enum {
    BOOL,
    STR,
} type_t;

typedef struct {
    type_t type;
    bool valid;
    void* content;
    const char* name;
    const char* description;
} flag_t;

flag_t* set_flag(const type_t type, const char* name, const char* description);
void filter_flags(int* argc, char** argv);
#endif

#ifndef FLAG_H_IMPLEMENTATION
#define FLAG_H_IMPLEMENTATION

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

// TODO: make better
//combination of cryptohash and lagrange might do the trick -> look into that -> every other func can be simplified
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

        //checking
        printf("%s\t%s\n", FLAG_BUFFER[i].name, (description) ? description : "");
    }
    printf("\n");
}

//finds right slot in global FLAG array, sets Flag and returns pointer to that slot
flag_t* set_flag(const type_t type, const char* name, const char* description)
{
    //name must exist
    ASSERT(name != NULL, "Flags need a name\n");

    //index where it should be
    size_t index = hash(name);

    //compensate collision
    while (FLAG_BUFFER[index].name != NULL)
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0; //flip from arr len to 0

    FLAG_BUFFER[index].name = name;
    FLAG_BUFFER[index].type = type;
    FLAG_BUFFER[index].description = description;

    return &FLAG_BUFFER[index];
}

flag_t* get_flag(const char* name)
{
    size_t index = hash(name);
    for (int k = 0; k < FLAG_CAPACITY; ++k) {
        if (strcmp(FLAG_BUFFER[index].name, name) == 0)
            return &FLAG_BUFFER[index];
        index = (index < FLAG_CAPACITY - 1) ? index + 1 : 0; //flip to index 0 if end of array is reached
    }
    return NULL;
}

// filters argv for flags and parameters, stores valid flags in global FLAG_BUFFER
// returns with exit code 1 if STR Flag doesn't have a parameter
void filter_flags(int* argc, char** argv)
{
    flag_t* flag = NULL;

    //new argc
    int rest_counter = 0;

    for (int i = 0; i < *argc; ++i) {

        //print descriptions when help flags occur
        if ((strcmp(argv[i], "-h") & strcmp(argv[i], "--help")) == 0) {
            dump_descriptions();
            continue;
        }

        //check if flag exists
        flag = get_flag(argv[i]);

        if (flag != NULL) {
            flag->valid = true;
            switch (flag->type) {
            case STR:
                //Next argv must be an argument for the flag
                ASSERT(i + 1 < *argc && get_flag(argv[i + 1]) == NULL, "ERROR: %s needs an argument\n", flag->name);

                flag->content = argv[++i];
                break;
            case BOOL:
                flag->content = (void*)&flag->valid; //little hacky | we need a void* ptr to a bool
                break;
            }

        } else
            //flag does not exist -> store argv[i] in argv
            argv[rest_counter++] = argv[i];
    }

    *argc = rest_counter;
}
#endif
