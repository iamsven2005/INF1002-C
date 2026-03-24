#include "headers.h"

int showall(struct Record *records, int total_records) {
    printf("\n");
    printf("Here are all the records found in the table \"StudentRecords\".\n");
    printf("\n");
    if (total_records == 0) {
        printf("No records found in the table \"StudentRecords\".\n");
        return -1;
    }

    printf("%-10s %-30s %-50s %-6s\n", "ID", "Name", "Programme", "Mark");
    for (int i = 0; i < 100; i++) {
        printf("%s","-");
        if (i == 99) {
            printf("\n");
        }
    }
    int i;
    for (i = 0; i < total_records; i++)
    {
        printf("%-10d %-30s %-50s %-6.1f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
    }
    for (int i = 0; i < 100; i++) {
        printf("%s","-");
        if (i == 99) {
            printf("\n");
        }
    }
    printf("Total Records: %d\n", total_records);

    return 0;
}
