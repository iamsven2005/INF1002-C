#include "open.h"
/*Opens databasse file, read each line and store the records in allocated array*/
int execute_on_file(struct Record **records, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file \"%s\".\n", filename);
        return -1;
    }
    
    int line_count = 0;
    int capacity = ARRAY_SIZE;
    *records = malloc(sizeof(struct Record) * capacity);

    if (*records == NULL) {
        printf("Failed to initialize record memory.\n");
        fclose(file);
        return -1;
    }

    char string[256];

    while (fgets(string, sizeof(string), file)) {
        string[strcspn(string, "\n")] = 0;
/*Expand record array when current capacity is full*/
        if (line_count >= capacity) {
            capacity += ARRAY_SIZE;
            struct Record *temp = realloc(*records, sizeof(struct Record) * capacity);
            if (temp == NULL) {
                printf("Failed to expand Record memory.\n");
                free(*records);
                *records = NULL;
                fclose(file);
                return -1;
            }
            *records = temp;
        }
    /*Splits one CSV line into ID, Name, Programme and Mark fields*/
        char *token = strtok(string, OPEN_DELIM);
        if (token != NULL) {
            (*records)[line_count].id = atoi(token);
        }
        else {
            printf("No delimiter %s for ID column", OPEN_DELIM);
            free(*records);
            *records = NULL;
            fclose(file);
            return -1;
        }

        token = strtok(NULL, OPEN_DELIM);
        if (token != NULL) {
           strncpy((*records)[line_count].name, token, sizeof((*records)->name));
           (*records)[line_count].name[sizeof((*records)[line_count].name) - 1] = '\0';
        }
        else {
            printf("No delimiter %s for Name column", OPEN_DELIM);
            free(*records);
            *records = NULL;
            fclose(file);
            return -1;
        }
        token = strtok(NULL, OPEN_DELIM);
        if (token != NULL) {
           strncpy((*records)[line_count].prog, token, sizeof((*records)->prog));
           (*records)[line_count].prog[sizeof((*records)[line_count].prog) - 1] = '\0';
        }
        else {
            printf("No delimiter %s for Prog column", OPEN_DELIM);
            free(*records);
            *records = NULL;
            fclose(file);
            return -1;
        }

        token = strtok(NULL, "\n");
        if (token != NULL) {
            (*records)[line_count].marks = atof(token);
        }
        else {
            printf("No delimiter %d for Marks column.\n", '\n');
            free(*records);
            *records = NULL;
            fclose(file);
            return -1;
        }

        line_count++;
    }

    fclose(file);

    return line_count;
}
