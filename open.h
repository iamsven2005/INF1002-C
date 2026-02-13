// Done by: Tristan Tan
#ifndef OPEN_H
#define OPEN_H

#define ARRAY_SIZE 1024 
#define OPEN_DELIM ","

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Record {
    int id;
    char name[100];
    char prog[256];
    float marks;

    bool has_id;
    bool has_name;
    bool has_prog;
    bool has_mark;

};

int open_and_read_file(struct Record **records, const char *filename);

#endif
