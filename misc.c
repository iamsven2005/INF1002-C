#include "misc.h"

void to_lower(char string[]){
    for (size_t i = 0; i < strlen(string); i++){
            string[i] = tolower((unsigned char) string[i]);
        }
}

int comp(const void *a, const void *b) {
    int *row1 = (int*)a;
    int *row2 = (int*)b;
    return (*row1 - *row2);
}

void help_menu(char *input) {
    printf("\n");
    if (input == NULL) {
        printf("Usage: HELP [COMMAND]\n");
        printf("Desc: Display help menu for specified command\n");
        printf("Commands:\n");
        printf("\tOPEN\n");
        printf("\tSHOW ALL\n");
        printf("\tINSERT\n");
        printf("\tQUERY\n");
        printf("\tUPDATE\n");
        printf("\tDELETE\n");
        printf("\tSAVE\n");
        printf("\tSNAPSHOT\n");
        printf("\tSORT\n");
        printf("\tSUMMARY\n");
    }
    else {
        if (strcmp(input, "open") == 0) {
            printf("Usage: OPEN\n");
            printf("Desc: Open database file. Database file must be opened before any other operations can be performed.\n");
        }
        else if (strcmp(input, "show all") == 0) {
            printf("Usage: SHOW ALL\n");
            printf("Desc: Displays all available records.\n");
        }
        else if (strcmp(input, "insert") == 0) {
            printf("Usage: INSERT [ARGUMENT]\n");
            printf("Desc: Insert a record into the database.\n");
            printf("\tOptional values must be provided together.\n");
            printf("Arguments:\n");
            printf("\t%-15sProvide ID value (Required).\n", "ID=");
            printf("\t%-15sProvide Name value (Optional).\n", "NAME=");
            printf("\t%-15sProvide Programme value (Optional).\n", "PROGRAMME=");
            printf("\t%-15sProvide Mark value (Optional).\n", "MARK=");
        }
        else if (strcmp(input, "query") == 0) {
            printf("Usage: QUERY [ARGUMENT]\n");
            printf("Desc: Queries a record in the database.\n");
            printf("Arguments:\n");
            printf("\t%-15sProvide ID to query.\n", "ID=");
        }
        else if (strcmp(input, "update") == 0) {
            printf("Usage: UPDATE [ARGUMENT]\n");
            printf("Desc: Updates a record in the database.\n");
            printf("Arguments:\n");
            printf("\t%-15sProvide ID of record to update (Required).\n", "ID=");
            printf("\t%-15sName to be updated\n", "Name=");
            printf("\t%-15sProgramme to be updated.\n", "Programme=");
            printf("\t%-15sMarks to be updated.\n","MARK=");
        }
        else if (strcmp(input, "delete") == 0) {
            printf("Usage: DELETE [ARGUMENT]\n");
            printf("Desc: Deletes a record from the database.\n");
            printf("Arguments:\n");
            printf("\t%-15sId of record to be deleted.\n", "ID=");
        }
        else if (strcmp(input, "save") == 0) {
            printf("Usage: SAVE\n");
        }
        else if (strcmp(input, "snapshot") == 0) {
            printf("Usage: [COMMAND] SNAPSHOT <file_name>\n");
            printf("Desc: Show, Create, delete, or restore snapshots.\n");
            printf("Commands:\n");
            printf("\t%-15sShow snapshots. Does not take file name as argument.\n", "SHOW");
            printf("\t%-15sCreate a snapshot of the current database file.\n", "CREATE");
            printf("\t%-15sRestores database to a previous snapshot.\n", "RESTORE");
            printf("\t%-15sDelete snapshot.\n", "DELETE");
        }
        else if (strcmp(input, "sort") == 0) {
            printf("Usage: SORT BY [ATTRIBUTE] [ORDER]");
            printf("Desc: Sort records by attribute and order.\n");
            printf("Attributes:\n");
            printf("\t%-15s", "ID");
            printf("\t%-15s", "MARKS");
            printf("Order:\n");
            printf("\t%-15sAscending order.\n", "ASC");
            printf("\t%-15sDescending order.\n", "DESC");
        }
        else if (strcmp(input, "summary") == 0) {
            printf("Usage: SHOWSUMMARY\n");
        }
        else {
            printf("No help menu available for command %s.\n", input);
        }
    }
}

int strcasecmp_ci(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower(*s1) != tolower(*s2)) {
            return tolower(*s1) - tolower(*s2);
        }
        s1++; s2++;
    }
    return tolower(*s1) - tolower(*s2);
}

int isValidID(const char *str, int *result) {
    if (!str || str[0] == '\0') return 0;
    char *endptr;
    *result = (int)strtol(str, &endptr, 10);
    return endptr != str && *endptr == '\0';
}

int isValidMarks(const char *str, float *result) {
    char *endptr;
    *result = strtof(str, &endptr);
    return endptr != str && *endptr == '\0';
}

int isValidName(const char *str) {
    if (!str || str[0] == '\0') return 0;
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

char* find_id_value(const char *input) {
    char *id_start = strstr(input, "ID=");
    if (!id_start) id_start = strstr(input, "id=");
    if (!id_start) id_start = strstr(input, "Id=");
    return id_start ? id_start + 3 : NULL;
}

int find_record_index(struct Record *records, int size, int id) {
    for (int i = 0; i < size; i++) {
        if (records[i].id == id) return i;
    }
    return -1;
}
