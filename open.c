// Done by: Tristan Tan
# include "open.h"

int open_and_read_file(struct Record **records, const char *filename) {
    FILE *file = fopen(filename, "r");
    
    //check if file opened successfully
    if (file == NULL) {
        printf("Error opening file"); 
        return -1;
    }
    
    // read file and store in struct
    int line_count = 0;
    *records = malloc(sizeof(struct Record) * ARRAY_SIZE);

    if (records == NULL) {
        printf("Failed to initialize record memory.\n");
        fclose(file);
        return -1;
    }

    char string[256];

    while (fgets(string, sizeof(string), file)) {
        // strip \n
        string[strcspn(string, "\n")] = 0;

        if (line_count > ARRAY_SIZE) { // rows exceeded allocated size
            struct Record *temp = realloc(*records, sizeof(struct Record) + ARRAY_SIZE);
            if (temp == NULL) {
                printf("Failed to expand Record memory.\n");
                fclose(file);
                return -1;
            }
            *records = temp;
        }
        // ID column
        char *token = strtok(string, OPEN_DELIM); //search for delim in current line
        if (token != NULL) {
            (*records)[line_count].id = atoi(token);
        }
        else {
            printf("No delimiter %s for ID column", OPEN_DELIM);
            return 1;
        }

        // Name column
        token = strtok(NULL, OPEN_DELIM);
        if (token != NULL) {
           strncpy((*records)[line_count].name, token, sizeof((*records)->name));
           (*records)[line_count].name[sizeof((*records)[line_count].name) - 1] = '\0'; // add null byte
        }
        else {
            printf("No delimiter %s for Name column", OPEN_DELIM);
            return -1;
        }
        // Programme column
        token = strtok(NULL, OPEN_DELIM);
        if (token != NULL) {
           strncpy((*records)[line_count].prog, token, sizeof((*records)->prog));
           (*records)[line_count].prog[sizeof((*records)[line_count].prog) - 1] = '\0'; // add null byte
        }
        else {
            printf("No delimiter %s for Prog column", OPEN_DELIM);
            return -1;
        }

        // Marks column delimiter = '\n' since last element of row
        token = strtok(NULL, "\n");
        if (token != NULL) {
            (*records)[line_count].marks = atof(token);
        }
        else {
            printf("No delimiter %d for Marks column.\n", '\n');
            return -1;
        }

        line_count++; // go to next line
    }

    /* TO FREE MEMORY USED BY ARRAY
    for (size_t i = 0; i < total_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    */

    fclose(file);

    return line_count;
}
