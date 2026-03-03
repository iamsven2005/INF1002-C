#ifndef MISC_H
#define MISC_H

#include "headers.h"

void to_lower(char string[]);
int comp(const void *a, const void *b);
void help_menu(char *token);

// Validation helpers
int strcasecmp_ci(const char *s1, const char *s2);
int isValidID(const char *str, int *result);
int isValidMarks(const char *str, float *result);
int isValidName(const char *str);
char* find_id_value(const char *input);
int find_record_index(struct Record *records, int size, int id);

#endif // !MISC_H
