# include "headers.h"
# include "insert.h"

int check_record_exists(int id, int *size, struct Record *records) {
    for (int i = 0; i < *size; i++) {
        if (records[i].id == id) {
            return 1;
        }
    }
    return 0;
}


bool insert(struct Record data, struct Record *records, int *records_size, char *token) {
    data.has_id = false;
    data.has_name = false;
    data.has_prog = false;
    data.has_mark = false;

    // Method: Getting the position of the "keys" of all attributes
    // then subtract from the next key position to get the value length
    // then value = key position + key length
    token = strtok(NULL, ""); // everything after INSERT
    // Validate attributes provided after INSERT
    if (token == NULL) {
        printf("No data provided for INSERT.\n");
        return true;
    }

    int token_len = strlen(token); // total length of values

    // starting position of each key
    char *id_key = strstr(token, "id=");
    char *name_key = strstr(token, "name=");
    char *prog_key = strstr(token, "programme=");
    char *mark_key = strstr(token, "mark=");

    // Validate ID field exists 
    if (id_key == NULL) {
        printf("ID field is mandatory.\n");
        return true;
    }

    // Confirm optional if optional values are provided
    // more efficient method since true 1 == true
    int optional_count = (name_key != NULL) + (prog_key != NULL) + (mark_key != NULL);
    if (optional_count > 0 && optional_count < 3) {
        printf("Name, Prog, and Mark fields must be provided together.\n");
        return true;
    }
    
    // Struct Array to store key_name and key_position
    struct Key {
        char *name;
        char *pos;
    } keys_array[4];

    keys_array[0].name = "id";;
    keys_array[0].pos = id_key;
    keys_array[1].name = "name";
    keys_array[1].pos = name_key;
    keys_array[2].name = "programme";
    keys_array[2].pos = prog_key;
    keys_array[3].name = "mark";
    keys_array[3].pos = mark_key;

    // Sort keys by position in input string
    // bubble Sort
    for (int i = 0; i < sizeof(keys_array)/sizeof(keys_array[0]) - 1; i++) {
        for (int j = 0; j < sizeof(keys_array)/sizeof(keys_array[0]) - i - 1; j++) {
            // check for NULL values
            if (keys_array[j].pos == NULL) continue;
            if (keys_array[j + 1].pos == NULL) continue;

            if (keys_array[j].pos > keys_array[j + 1].pos) {
                // swap with adjacent
                struct Key temp = keys_array[j];
                keys_array[j] = keys_array[j + 1];
                keys_array[j + 1] = temp;
            }

        }
    }

    // what im tryna do is get the value 
    // to do that i need to know the length of the value
    // key_pos = start pos of key
    // value_pos = start pos of value
    // so value_pos - key_pos - 1 = length of value (whitespace)
    // then value = key_pos + key_len
    // then value will be value for that key (key name is in 2D array)
    // loop through each attribute
    for (int i = 0; i < (sizeof(keys_array)/sizeof(keys_array[0])); i++) {
        char *key_name = keys_array[i].name; 
        char * key_pos = keys_array[i].pos;

        // skip if key not provided
        if (key_pos == NULL) continue;

        char *value_pos = key_pos + strlen(key_name) + 1; // account for "=";
        int value_len;

        // next key pos
        char *next_key_pos = NULL;
        for (int j = i + 1; j < (sizeof(keys_array)/sizeof(keys_array[0])); j++) {
            if (keys_array[j].pos != NULL) {
                next_key_pos = keys_array[j].pos;
                break;
            }
        }

        if (next_key_pos == NULL) {
            value_len = token_len - (value_pos - token); // EOL - last value pos
        }
        else {
            value_len = next_key_pos - value_pos - 1; // -1 for whitespace
        }

        #define MAX_VALUE_LEN 50 // follow max len of program since it is the longest
        char value[MAX_VALUE_LEN + 1]; // account for null byte
        
        if (value_len > MAX_VALUE_LEN) {
            return true;
        }
        
        strncpy(value, value_pos, value_len);
        value[value_len] = '\0';

        // if value empty
        if (strlen(value) == 0) {
            printf("Value for %s not provided.\n", 
                    strcmp(key_name, "id") == 0 ? "ID":
                    strcmp(key_name, "name") == 0 ? "NAME":
                    strcmp(key_name, "programme") == 0 ? "PROGRAMME":
                    strcmp(key_name, "mark") == 0 ? "MARK": "UNKNOWN"); // claude showed rlly cool this ternary operator trick
            return true;
        }
        // now that we have key name and value
        // can add to structure
        if (strcmp(key_name, "id") == 0) {
            for (int i = 0; i < strlen(value); i++) {
                if (!isdigit(value[i])) {
                    printf("ID should only have integers.\n");
                    return true;
                }
            }
            data.id = atoi(value);
            data.has_id = true;
        }
        else if (strcmp(key_name, "name") == 0) {
            // different method to get value to account for spaces
            /* Idea: Get all */

            if (strlen(value) > 30) {
                printf("Name max 30 characters.\n");
                return true;
            }
            strncpy(data.name, value, 29);
            data.name[29] = 0;
            data.has_name = true;
        }
        else if (strcmp(key_name, "programme") == 0) {
            if (strlen(value) > 50) {
                printf("Programme max 50 characters.\n");
                return true;
            }
            strncpy(data.prog, value, 50);
            data.prog[50] = 0; 
            data.has_prog = true;
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
            data.marks = mark_value;
            data.has_mark = true;
        }
        else {
            printf("Only ID, NAME, PROGRAMME, and MARK fields allowed.\n");
            return true;
        }
    } // end of loop

   
    if ((check_record_exists(data.id, records_size, records) == 1)) {
        printf("The Record with ID=%d already exists.\n", data.id);
        return true;
    }
    else {
        if (optional_count != 3) {
            printf("The Record with ID=%d does not exists.\n", data.id);
            return true;
        }
    }

    // capitalize name and prog
    // first ensure there is data to capitalize
    if (data.has_name) {
        data.name[0] = toupper((unsigned char) data.name[0]);

        for (int i = 1; i <strlen(data.name); i++) {
            if (data.name[i - 1] == ' ') {
                data.name[i] = toupper((unsigned char) data.name[i]);
            }
        } 
    }
    if (data.has_prog){
        data.prog[0] = toupper((unsigned char) data.prog[0]);

        for (int i = 1; i <strlen(data.prog); i++) {
            if (data.prog[i - 1] == ' ') {
                data.prog[i] = toupper((unsigned char) data.prog[i]);
            }
        }
    }


    // alls good then insert record
    // finally can insert new user
    records[*records_size] = data; 
    (*records_size)++; // increament to match new record size

    if (records[*records_size - 1].id == data.id) {
        printf("A new record with ID=%d is successfully inserted.\n", data.id);
    }
    else {
        perror("Something went wrong while inserting record.\n");
        return 1;
    }


    return false;
}

