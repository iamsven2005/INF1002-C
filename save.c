#include "headers.h"

int save(struct Record *records, int total_records, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Failed to save file '%s'\n", filename);
        return -1;
    }
    int i;
    for (i = 0; i < total_records; i++)
    {
        fprintf(file, "%d,%s,%s,%.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
    }
    fclose(file);
    printf("The database file \"%s\" is successfully saved.\n", filename);

    return 0;
}
