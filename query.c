#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers.h"
#include "query.h"

// Helper function to validate integer ID
int isValidID_query(const char* str, int* result) {
    if (str[0] == '\0') return 0;
    char* endptr;
    *result = (int)strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    return 1;
}

void query(struct Record* records, int records_size, const char* input) {
    if (records_size == 0) {
        printf("No records available to query.\n");
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
        printf("Invalid command. Follow the format: QUERY ID=<ID>\n");
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
    if (!isValidID_query(id_str, &search_id)) {
        printf("Invalid ID.\n");
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

    // Display the record in table format (edit later to follow max length of inputs)
    printf("The record with ID=%d is found in the data table\n", search_id);
    printf("%-10s %-30s %-50s%-6s\n", "ID", "Name", "Programme", "Mark");
    printf("%-10d %-30s %-50s %-6.2f\n",
        records[index].id,
        records[index].name,
        records[index].prog,
        records[index].marks);
}
