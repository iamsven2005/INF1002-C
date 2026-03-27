#ifndef MISC_H
#define MISC_H

#include "headers.h"

void to_lower(char string[]);
int compare_ints(const void *a, const void *b);
void help_menu(char *token);
char* find_substring_ci(const char *haystack, const char *needle);

// Validation helpers
int strcasecmp_ci(const char *s1, const char *s2);
int isValidID(const char *id_text, int *result);
int isValidMarks(const char *mark_text, float *result);
int isValidName(const char *name_text);
char* find_id_value(const char *input);
int find_record_index(struct Record *records, int size, int id);

#endif // !MISC_H
