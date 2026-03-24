#define _CRT_SECURE_NO_WARNINGS
#include "headers.h"
#include "delete.h"
/*Deletes a record by student ID*/
void delete_record(struct Record records[], int* records_size, const char* input) {
    if (*records_size == 0) {
        printf("No records available to delete.\n");
        return;
    }

    char *id_value_start = find_id_value(input);
    if (!id_value_start) {
        printf("Invalid command. Follow the format: DELETE ID=<ID>\n");
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
        printf("Invalid ID.\n");
        return;
    }

    int index = find_record_index(records, *records_size, search_id);

    if (index == -1) {
        printf("The record with ID=%d does not exist.\n", search_id);
        return;
    }

    char confirm[10];
    printf("Are you sure you want to delete record with ID=%d? Type \"Y\" to Confirm or type \"N\" to cancel.\n", search_id);
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';

    if (strcasecmp_ci(confirm, "y") != 0 && strcasecmp_ci(confirm, "yes") != 0) {
        printf("The deletion is cancelled.\n");
        return;
    }
/*Replaces the deleted record with the later records*/
    for (int j = index; j < (*records_size - 1); j++) {
        records[j] = records[j + 1];
    }
    (*records_size)--;

    printf("The record with ID=%d is successfully deleted.\n", search_id);
}
