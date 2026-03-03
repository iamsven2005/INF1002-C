#define _CRT_SECURE_NO_WARNINGS
#include "headers.h"
#include "update.h"

void updateRecord(struct Record records[], int records_size, const char* input) {
    if (records_size == 0) {
        printf("No records available to update.\n");
        return;
    }

    char *id_value_start = find_id_value(input);
    if (!id_value_start) {
        printf("Missing ID. Usage: UPDATE ID=<id> <Field>=<Value>\n");
        return;
    }

    char id_str[20];
    int i = 0;
    while (*id_value_start && *id_value_start != ' ' && i < 19) {
        id_str[i++] = *id_value_start++;
    }
    id_str[i] = '\0';

    int search_id;
    if (!isValidID(id_str, &search_id)) {
        printf("Invalid ID format. ID must be an integer.\n");
        return;
    }

    int index = find_record_index(records, records_size, search_id);

    if (index == -1) {
        printf("The record with ID=%d does not exist.\n", search_id);
        return;
    }

    char* field_start = id_value_start;
    while (*field_start == ' ') field_start++;

    if (*field_start == '\0') {
        printf("Missing field. Usage: UPDATE ID=<id> <Field>=<Value>\n");
        return;
    }

    char* eq_ptr = strchr(field_start, '=');
    if (eq_ptr == NULL) {
        printf("Missing field value.\n");
        return;
    }

    char field[20];
    int len = eq_ptr - field_start;
    if (len >= 20) len = 19;
    strncpy(field, field_start, len);
    field[len] = '\0';

    char* value = eq_ptr + 1;

    if (*value == '\0') {
        printf("Missing value for field '%s'.\n", field);
        return;
    }

    if (strcasecmp_ci(field, "Name") == 0) {
        if (!isValidName(value)) {
            printf("Invalid Name. Name must contain only letters and spaces.\n");
            return;
        }
        strncpy(records[index].name, value, sizeof(records[index].name) - 1);
        records[index].name[sizeof(records[index].name) - 1] = '\0';
    }
    else if (strcasecmp_ci(field, "Programme") == 0) {
        if (!isValidName(value)) {
            printf("Invalid Programme. Programme must contain only letters and spaces.\n");
            return;
        }
        strncpy(records[index].prog, value, sizeof(records[index].prog) - 1);
        records[index].prog[sizeof(records[index].prog) - 1] = '\0';
    }
    else if (strcasecmp_ci(field, "Mark") == 0) {
        float newMark;
        if (!isValidMarks(value, &newMark)) {
            printf("Invalid Mark. Mark must be a valid number.\n");
            return;
        }
        if (newMark < 0 || newMark > 100) {
            printf("Invalid Mark. Mark must be between 0 and 100.\n");
            return;
        }
        records[index].marks = newMark;
    }
    else {
        printf("Unknown field '%s'. Valid fields: Name, Programme, Mark\n", field);
        return;
    }

    printf("The record with ID=%d is successfully updated.\n", search_id);
}
