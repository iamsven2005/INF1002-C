// Done By: Tristan Tan
#ifndef INSERT_H
#define INSERT_H

#include <stdbool.h>

int check_record_exists(int id, int *size, struct Record *records);

bool insert(struct Record data, struct Record *records, int *records_size, char *token);

#endif // !INSERT_H
