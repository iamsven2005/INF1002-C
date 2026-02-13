#include "headers.h"

int save(struct Record *records, int total_records)
{
    // open file in write mode
    FILE *file = fopen(FILENAME, "w");

    // check if file open successful
    if (file == NULL)
    {
        printf("Error: Failed to save file '%s'\n", FILENAME);
        return -1;
    }

    // write record to file following standard format
    int i;
    for (i = 0; i < total_records; i++)
    {
        fprintf(file, "%d,%s,%s,%.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
    }

    // close
    fclose(file);

    // tell user save successful
    printf("Successfully saved %d records to '%s'\n", total_records, FILENAME);

    return 0;
}
