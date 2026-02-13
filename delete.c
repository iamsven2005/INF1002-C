#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers.h"
#include "delete.h"

// Helper function for case-insensitive string comparison
int strcasecmp_delete(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (tolower(*s1) != tolower(*s2)) {
            return tolower(*s1) - tolower(*s2);
        }
        s1++;
        s2++;
    }
    return tolower(*s1) - tolower(*s2);
}

// Helper function to validate integer ID
int isValidID_delete(const char* str, int* result) {
    if (str[0] == '\0') return 0;
    char* endptr;
    *result = (int)strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

void delete_record(struct Record records[], int* records_size, const char* input) {
    if (*records_size == 0) {
        printf("No records available to delete.\n");
        return;
    }

    // Parse the input command
    // Format: ID=<id>
    char command[256];
    strncpy(command, input, sizeof(command) - 1);
    command[sizeof(command) - 1] = '\0';

    // Check for ID= (case-insensitive)
    char* id_start = strstr(command, "ID=");
    if (id_start == NULL) {
        id_start = strstr(command, "id=");
    }
    if (id_start == NULL) {
        id_start = strstr(command, "Id=");
    }
    if (id_start == NULL) {
        printf("Invalid command. Follow the format: DELETE ID=<ID>\n");
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
    if (!isValidID_delete(id_str, &search_id)) {
        printf("Invalid ID.\n");
        return;
    }

    // Find the record
    int index = -1;
    for (int j = 0; j < *records_size; j++) {
        if (records[j].id == search_id) {
            index = j;
            break;
        }
    }

    if (index == -1) {
        printf("The record with ID=%d does not exist.\n", search_id);
        return;
    }

    // Confirm deletion
    char confirm[10];
    printf("Are you sure you want to delete record with ID=%d? (y/n): ", search_id);
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';

    if (strcasecmp_delete(confirm, "y") != 0 && strcasecmp_delete(confirm, "yes") != 0) {
        printf("Cancelled.\n");
        return;
    }

    // Shift elements left to overwrite the deleted entry
    for (int j = index; j < (*records_size - 1); j++) {
        records[j] = records[j + 1];
    }
    (*records_size)--;

    printf("The record with ID=%d is successfully deleted.\n", search_id);
}
