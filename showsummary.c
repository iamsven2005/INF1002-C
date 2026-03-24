#include "headers.h"

int showsummary(struct Record *records, int total_records) {
    if (total_records == 0) {
        printf("Error, No Records Found! \n");
        return -1;
    }
    
    printf("Total number of students: %d\n", total_records);
    
    int i;
    float total = 0;
    float average = 0;
    for (i = 0; i < total_records; i++)
    {
        total += records[i].marks;
        average = total / total_records;
    }
    printf("Average marks: %.1f\n", average);

    int highest = 0;
    int lowest = 0;

    for (i = 1; i < total_records; i++)
    {
        if (records[i].marks > records[highest].marks)
        {
            highest = i;
        }
        if (records[i].marks < records[lowest].marks)
        {
            lowest = i;
        }
    }

    printf("Highest mark: %.1f - %s\n", records[highest].marks, records[highest].name);
    printf("Lowest mark: %.1f - %s\n", records[lowest].marks, records[lowest].name);
    printf("\n");
    return 0;
}
