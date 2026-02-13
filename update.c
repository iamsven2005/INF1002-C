#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers.h"
#include "update.h"

// Helper function for case-insensitive string comparison
int strcasecmp_custom(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (tolower(*s1) != tolower(*s2)) {
            return tolower(*s1) - tolower(*s2);
        }
        s1++;
        s2++;
    }
    return tolower(*s1) - tolower(*s2);
}

// Helper function to check if string contains only letters and spaces
int isValidName(const char* str) {
    if (str[0] == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

// Helper function to validate marks input
int isValidMarks(const char* str, float* result) {
    char* endptr;
    *result = strtof(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

// Helper function to validate integer ID
int isValidID(const char* str, int* result) {
    char* endptr;
    *result = (int)strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

void updateRecord(struct Record records[], int records_size, const char* input) {
    if (records_size == 0) {
        printf("No records available to update.\n");
        return;
    }

    // Parse the input command
    // Format: ID=<id> <Field>=<Value>
    char command[256];
    strncpy(command, input, sizeof(command) - 1);
    command[sizeof(command) - 1] = '\0';

    // Check for ID= (try both uppercase and lowercase)
    char* id_start = strstr(command, "ID=");
    if (id_start == NULL) {
        id_start = strstr(command, "id=");
    }
    if (id_start == NULL) {
        printf("Missing ID. Usage: UPDATE ID=<id> <Field>=<Value>\n");
        return;
    }

    // Extract ID value
    char* id_value_start = id_start + 3; // Skip "ID="
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

    // Find the record
    int index = -1;
    for (int j = 0; j < records_size; j++) {
        if (records[j].id == search_id) {
            index = j;
            break;
        }
    }

    if (index == -1) {
        printf("The record with ID=%d does not exist.\n", search_id);
        return;
    }

    // Move to field=value part (skip whitespace after ID)
    char* field_start = id_value_start;
    while (*field_start == ' ') field_start++;

    // Check if there's a field to update
    if (*field_start == '\0') {
        printf("Missing field. Usage: UPDATE ID=<id> <Field>=<Value>\n");
        return;
    }

    // Find the '=' in field=value
    char* eq_ptr = strchr(field_start, '=');
    if (eq_ptr == NULL) {
        printf("Missing field value.\n");
        return;
    }

    // Extract field name
    char field[20];
    int len = eq_ptr - field_start;
    if (len >= 20) len = 19;
    strncpy(field, field_start, len);
    field[len] = '\0';

    // Extract value (everything after '=')
    char* value = eq_ptr + 1;

    // Check if value is empty
    if (*value == '\0') {
        printf("Missing value for field '%s'.\n", field);
        return;
    }

    // Update based on field (case-insensitive)
    if (strcasecmp_custom(field, "Name") == 0) {
        if (!isValidName(value)) {
            printf("Invalid Name. Name must contain only letters and spaces.\n");
            return;
        }
        strncpy(records[index].name, value, sizeof(records[index].name) - 1);
        records[index].name[sizeof(records[index].name) - 1] = '\0';
    }
    else if (strcasecmp_custom(field, "Programme") == 0) {
        if (!isValidName(value)) {
            printf("Invalid Programme. Programme must contain only letters and spaces.\n");
            return;
        }
        strncpy(records[index].prog, value, sizeof(records[index].prog) - 1);
        records[index].prog[sizeof(records[index].prog) - 1] = '\0';
    }
    else if (strcasecmp_custom(field, "Mark") == 0) {
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
