# include "headers.h"
# include "insert.h"
/*Checks if theres a record with the same Student ID*/
int check_record_exists(int id, int *size, struct Record *records) {
    for (int i = 0; i < *size; i++) {
        if (records[i].id == id) {
            return 1;
        }
    }
    return 0;
}

/* Insets new record, validate fields and storing the record in memory*/
bool insert(struct Record new_record, struct Record *records, int *records_size, char *args) {
    new_record.has_id = false;
    new_record.has_name = false;
    new_record.has_prog = false;
    new_record.has_mark = false;

    // Method: Getting the position of the "keys" of all attributes
    // then subtract from the next key position to get the value length
    // then value = key position + key length
    if (args == NULL || *args == '\0') {
        printf("No data provided for INSERT.\n");
        return true;
    }

    int token_len = strlen(args);

    char *id_key = find_substring_ci(args, "id=");
    char *name_key = find_substring_ci(args, "name=");
    char *prog_key = find_substring_ci(args, "programme=");
    char *mark_key = find_substring_ci(args, "mark=");

    if (id_key == NULL) {
        printf("ID field is mandatory.\n");
        return true;
    }

    int optional_count = (name_key != NULL) + (prog_key != NULL) + (mark_key != NULL);
    if (optional_count > 0 && optional_count < 3) {
        printf("Name, Prog, and Mark fields must be provided together.\n");
        return true;
    }
    /*stores detected fields and their positions, easier to identify*/
    struct Key {
        char *field_name;
        char *field_position;
    } keys_array[4];
    keys_array[0].field_name = "id";;
    keys_array[0].field_position = id_key;
    keys_array[1].field_name = "name";
    keys_array[1].field_position = name_key;
    keys_array[2].field_name = "programme";
    keys_array[2].field_position = prog_key;
    keys_array[3].field_name = "mark";
    keys_array[3].field_position = mark_key;
    for (int i = 0; i < sizeof(keys_array)/sizeof(keys_array[0]) - 1; i++) {
        for (int j = 0; j < sizeof(keys_array)/sizeof(keys_array[0]) - i - 1; j++) {
            if (keys_array[j].field_position == NULL) continue;
            if (keys_array[j + 1].field_position == NULL) continue;

            if (keys_array[j].field_position > keys_array[j + 1].field_position) {
                struct Key temp = keys_array[j];
                keys_array[j] = keys_array[j + 1];
                keys_array[j + 1] = temp;
            }

        }
    }
/*Extracts and validates the value for each provided field.*/
    for (int i = 0; i < (sizeof(keys_array)/sizeof(keys_array[0])); i++) {
        char *key_name = keys_array[i].field_name; 
        char *key_pos = keys_array[i].field_position;
        if (key_pos == NULL) continue;
        char *value_pos = key_pos + strlen(key_name) + 1;
        int value_len;
        char *next_key_pos = NULL;
        for (int j = i + 1; j < (sizeof(keys_array)/sizeof(keys_array[0])); j++) {
            if (keys_array[j].field_position != NULL) {
                next_key_pos = keys_array[j].field_position;
                break;
            }
        }

        if (next_key_pos == NULL) {
            value_len = token_len - (value_pos - args);
        }
        else {
            value_len = next_key_pos - value_pos - 1;
        }

        #define MAX_VALUE_LEN 50
        char value[MAX_VALUE_LEN + 1];
        
        if (value_len > MAX_VALUE_LEN) {
            return true;
        }
        
        strncpy(value, value_pos, value_len);
        value[value_len] = '\0';

        if (strlen(value) == 0) {
            printf("Value for %s not provided.\n", 
                    strcmp(key_name, "id") == 0 ? "ID":
                    strcmp(key_name, "name") == 0 ? "NAME":
                    strcmp(key_name, "programme") == 0 ? "PROGRAMME":
                    strcmp(key_name, "mark") == 0 ? "MARK": "UNKNOWN");
            return true;
        }
        if (strcmp(key_name, "id") == 0) {
            for (int i = 0; i < strlen(value); i++) {
                if (!isdigit(value[i])) {
                    printf("ID should only have integers.\n");
                    return true;
                }
            }
            new_record.id = atoi(value);
            new_record.has_id = true;
        }
        else if (strcmp(key_name, "name") == 0) {
            if (strlen(value) > 30) {
                printf("Name max 30 characters.\n");
                return true;
            }
            strncpy(new_record.name, value, 29);
            new_record.name[29] = 0;
            new_record.has_name = true;
        }
        else if (strcmp(key_name, "programme") == 0) {
            if (strlen(value) > 50) {
                printf("Programme max 50 characters.\n");
                return true;
            }
            strncpy(new_record.prog, value, 50);
            new_record.prog[50] = 0; 
            new_record.has_prog = true;
        }
        else if (strcmp(key_name, "mark") == 0) {
            char* endptr;
            float mark_value = strtof(value , &endptr);

            if (endptr == value || *endptr != '\0') {
                printf("MARK should only have numeric values.\n");
                return true;
            } 
            if (mark_value < 0 || mark_value > 100) {
                printf("Marks should be between 0 to 100.\n");
                return true;
            }
            new_record.marks = mark_value;
            new_record.has_mark = true;
        }
        else {
            printf("Only ID, NAME, PROGRAMME, and MARK fields allowed.\n");
            return true;
        }
    }
    /*Rejects insert if Student ID already exists in the current records.*/
    if ((check_record_exists(new_record.id, records_size, records) == 1)) {
        printf("The record with ID=%d already exists.\n", new_record.id);
        return true;
    }
    /*IF only ID is given, prompts the user to enter the remaining fields*/
    else {
        if (optional_count == 0) {
            char name_input[sizeof(new_record.name)];
            char prog_input[sizeof(new_record.prog)];
            char mark_input[32];
            char *endptr = NULL;

            printf("Enter Name: ");
            if (fgets(name_input, sizeof(name_input), stdin) == NULL) {
                printf("Invalid Name input.\n");
                return true;
            }
            name_input[strcspn(name_input, "\n")] = '\0';
            if (strlen(name_input) == 0 || strlen(name_input) > 30) {
                printf("Name max 30 characters.\n");
                return true;
            }

            printf("Enter Programme: ");
            if (fgets(prog_input, sizeof(prog_input), stdin) == NULL) {
                printf("Invalid Programme input.\n");
                return true;
            }
            prog_input[strcspn(prog_input, "\n")] = '\0';
            if (strlen(prog_input) == 0 || strlen(prog_input) > 50) {
                printf("Programme max 50 characters.\n");
                return true;
            }

            printf("Enter Mark: ");
            if (fgets(mark_input, sizeof(mark_input), stdin) == NULL) {
                printf("Invalid Mark input.\n");
                return true;
            }
            mark_input[strcspn(mark_input, "\n")] = '\0';
            new_record.marks = strtof(mark_input, &endptr);
            if (endptr == mark_input || *endptr != '\0' || new_record.marks < 0 || new_record.marks > 100) {
                printf("Marks should be between 0 to 100.\n");
                return true;
            }

            strncpy(new_record.name, name_input, sizeof(new_record.name) - 1);
            new_record.name[sizeof(new_record.name) - 1] = '\0';
            strncpy(new_record.prog, prog_input, sizeof(new_record.prog) - 1);
            new_record.prog[sizeof(new_record.prog) - 1] = '\0';

            new_record.has_name = true;
            new_record.has_prog = true;
            new_record.has_mark = true;
        }
        else if (optional_count != 3) {
            printf("Name, Prog, and Mark fields must be provided together.\n");
            return true;
        }
    }
/*Stores new record and increase the total record count*/
    records[*records_size] = new_record; 
    (*records_size)++;

    if (records[*records_size - 1].id == new_record.id) {
        printf("A new record with ID=%d is successfully inserted.\n", new_record.id);
    }
    else {
        perror("Something went wrong while inserting record.\n");
        return 1;
    }


    return false;
}
