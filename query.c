#define _CRT_SECURE_NO_WARNINGS
#include "headers.h"
#include "query.h"
/*Search for a record by student ID and display if found*/
void query(struct Record* records, int records_size, const char* input) {
    if (records_size == 0) {
        printf("No records available to query.\n");
        return;
    }
/*Extract the ID value*/
    char *id_value_start = find_id_value(input);
    if (!id_value_start) {
        printf("Invalid command. Follow the format: QUERY ID=<ID>\n");
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

    int index = find_record_index(records, records_size, search_id);

    if (index == -1) {
        printf("The record with ID=%d does not exist.\n", search_id);
        return;
    }

    printf("The record with ID=%d is found in the data table.\n", search_id);
    printf("%-10s %-30s %-50s%-6s\n", "ID", "Name", "Programme", "Mark");
    printf("%-10d %-30s %-50s %-6.1f\n", records[index].id, records[index].name, records[index].prog, records[index].marks);
}
