#include "headers.h"

int showall(struct Record *records, int total_records)
{
    // header
    printf("\n");
    printf("All Records: \n");
    printf("\n");

    // check if anything in record file
    if (total_records == 0)
    {
        printf("Error, No Records Found! \n");
        return -1;
    }

    // print table header
    printf("%-10s %-30s %-50s %-6s\n", "ID", "Name", "Program,", "Marks");
    for (int i = 0; i < 100; i++) {
        printf("%s","-");
        if (i == 99) {
            printf("\n");
        }
    }


    // arr record in table
    int i;
    for (i = 0; i < total_records; i++)
    {
        printf("%-10d %-30s %-50s %-6.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
    }

    // footer
    for (int i = 0; i < 100; i++) {
        printf("%s","-");
        if (i == 99) {
            printf("\n");
        }
    }

    // tabulate record number for easier viewing
    printf("Total Records: %d\n", total_records);

    return 0;
}
