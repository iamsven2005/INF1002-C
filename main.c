#define _CRT_SECURE_NO_WARNINGS
#include "headers.h"

struct Record *records = NULL;
int records_size = 0;

int main(){
    /* === DECLARTION ===*/
    printf("%55s\n", "Declaration");
    printf("%s",
        "SIT’s policy on copying does not allow the students to copy source code as well as assessment solutions\n"
        "from another person or other places. It is the students’ responsibility to guarantee that their\n"
        "assessment solutions are their own work. Meanwhile, the students must also ensure that their work is\n"
        "not accessible by others. Where such plagiarism is detected, both of the assessments involved will\n"
        "receive ZERO mark.\n\n"
        "We hereby declare that:\n"
        "\u2022 We fully understand and agree to the abovementioned plagiarism policy.\n"
        "\u2022 We did not copy any code from others or from other places.\n"
        "\u2022 We did not share our codes with others or upload to any other places for public access and will not do that in the future.\n"
        "\u2022 We agree that our project will receive Zero mark if there is any plagiarism detected.\n"
        "\u2022 We agree that we will not disclose any information or material of the group project to others or upload to any other places for public access.\n"
        "\u2022 We agree that we did not copy any code directly from generated sources\n\n"
        "Declared by: INF1002 P(Group9)\n"
        "Team members:\n"
        "1. Tan Sven\n"
        "2. Fong Jun\n"
        "3. Ethan Pang\n"
        "4. Choo Kai Jun\n"
        );
    printf("Date: %s\n\n", DATE_COMPLETED);

    /* === CMS START === */
    printf("%c", '*');
    for (int i = 0; i < 50; i++) {
        printf("%c", '-');
    }
    printf("%s", "Welcome to the CMS");
    for (int i = 0; i < 50; i++) {
        printf("%c", '-');
    }
    printf("%c\n", '*');

    /* === MAIN FUNCTION === */

    bool file_opened = false;
    char current_filename[260];
    strncpy(current_filename, FILENAME, sizeof(current_filename) - 1);
    current_filename[sizeof(current_filename) - 1] = '\0';
#undef MAX_INPUT
#define MAX_INPUT 250
    do {
        char input[MAX_INPUT];
        bool success = false;

        do {
            printf("%s: ", USER);
            fgets(input, sizeof(input), stdin);
            printf("%s: ", CMS);
            int len = strlen(input);
            if (len > 0 && input[len - 1] != '\n') {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Input too long, max %d characters\n", MAX_INPUT - 2);
                continue;
            }

            if (len == 1) {
                printf("Enter a command.\n");
                continue;
            }

            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }

            success = true;
        } while (!success);

        to_lower(input);
        char *input_copy = strdup(input);

        if (input_copy == NULL) {
            printf("Input duplicate failed.\n");
            return 1;
        }

        char *token = strtok(input_copy, " ");
        if (token == NULL) {
            printf("Enter a command.\n");
            free(input_copy);
            continue;
        }

        if (strcmp(token, "exit") == 0) {
            printf("Exiting...Goodbye :)\n");
            if (file_opened) {
                free(input_copy);
                free(records);
            }
            return 0;
        }
        else if (strcmp(token, "help") == 0) {
            char *arg = strtok(NULL, " \n");
            help_menu(arg);
            free(input_copy);
            continue;
        }
        else if (strcmp(token, "open") == 0) {
            char *arg = strtok(NULL, "");
            if (arg != NULL) {
                while (*arg == ' ') arg++;
                if (*arg != '\0') {
                    strncpy(current_filename, arg, sizeof(current_filename) - 1);
                    current_filename[sizeof(current_filename) - 1] = '\0';
                }
            }

            if (file_opened && records != NULL) {
                free(records);
                records = NULL;
                records_size = 0;
            }

            // call open file func
            records_size = execute_on_file(&records, current_filename);

            if (!records) {
                printf("Failed to read from database file \"%s\".\n", current_filename);
                free(input_copy);
                continue;
            }
            if (records_size < 0) {
                printf("Invalid record size.\n");
                free(input_copy);
                continue;
            }

            printf("The database file \"%s\" is successfully opened.\n", current_filename);
            file_opened = true;
            free(input_copy);
            continue;
        }
        if (strstr(input, "snapshot") != NULL) {
            token = strtok(NULL, " \n");
            if (strcmp(token, "snapshot") != 0){
                printf("Unknown command. Use HELP to see help menu.\n");
                continue;
            }
            char cwd[512] = "";
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("Error getting current working directory");
                free(input_copy);
                continue;
            }

            char dir_path[PATH_MAX] = "";
            if (!construct_snapshot_dir(cwd, dir_path, sizeof(dir_path))) {
                printf("Error: Directory path too long.\n");
                free(input_copy);
                continue;
            }

            char *snapshot_ptr = NULL;
            char *snapshot_name = NULL;

            if ((snapshot_ptr = strstr(input, "show snapshot")) != NULL) {
                printf("\nAvailable snapshots:\n");
                file_in_dir(dir_path, NULL);
                free(input_copy);
                continue;
            }
            else if ((snapshot_ptr = strstr(input, "create snapshot")) != NULL) {
                snapshot_name = snapshot_ptr + strlen("create snapshot ");
                while (*snapshot_name == ' ') snapshot_name++;
                char *end = snapshot_name + strlen(snapshot_name) - 1;
                while (end > snapshot_name && (*end == ' ' || *end == '\n')) {
                    *end = '\0';
                    end--;
                }

                if (!is_valid_snapshot_name(snapshot_name)) {
                    free(input_copy);
                    continue;
                }

                char file_path[PATH_MAX] = "";
                if (snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, snapshot_name) >= sizeof(file_path)) {
                    printf("Error: File path too long.\n");
                    free(input_copy);
                    continue;
                }

                create_snapshot(snapshot_name, file_path);
                free(input_copy);
                continue;
            }
            else if ((snapshot_ptr = strstr(input, "restore snapshot")) != NULL) {
                snapshot_name = snapshot_ptr + strlen("restore snapshot ");
                while (*snapshot_name == ' ') snapshot_name++;
                char *end = snapshot_name + strlen(snapshot_name) - 1;
                while (end > snapshot_name && (*end == ' ' || *end == '\n')) {
                    *end = '\0';
                    end--;
                }

                if (strlen(snapshot_name) == 0) {
                    printf("Snapshot name not provided.\n");
                    free(input_copy);
                    continue;
                }

                if (!is_valid_snapshot_name(snapshot_name)) {
                    free(input_copy);
                    continue;
                }

                if (!file_in_dir(dir_path, snapshot_name)) {
                    printf("Snapshot \"%s\" not found in snapshots folder.\n", snapshot_name);
                    free(input_copy);
                    continue;
                }

                char file_path[PATH_MAX] = "";
                if (snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, snapshot_name) >= sizeof(file_path)) {
                    printf("Error: File path too long.\n");
                    free(input_copy);
                    continue;
                }

                restore_snapshot(snapshot_name, file_path);
                free(input_copy);
                continue;
            }
            else if ((snapshot_ptr = strstr(input, "delete snapshot")) != NULL) {
                snapshot_name = snapshot_ptr + strlen("delete snapshot ");
                while (*snapshot_name == ' ') snapshot_name++;
                char *end = snapshot_name + strlen(snapshot_name) - 1;
                while (end > snapshot_name && (*end == ' ' || *end == '\n')) {
                    *end = '\0';
                    end--;
                }

                if (strlen(snapshot_name) == 0) {
                    printf("Snapshot name not provided.\n");
                    free(input_copy);
                    continue;
                }

                if (!is_valid_snapshot_name(snapshot_name)) {
                    free(input_copy);
                    continue;
                }

                if (!file_in_dir(dir_path, snapshot_name)) {
                    printf("Snapshot \"%s\" not found in snapshots folder.\n", snapshot_name);
                    free(input_copy);
                    continue;
                }

                char file_path[PATH_MAX] = "";
                if (snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, snapshot_name) >= sizeof(file_path)) {
                    printf("Error: File path too long.\n");
                    free(input_copy);
                    continue;
                }

                delete_snapshot(snapshot_name, file_path);
                free(input_copy);
                continue;
            }
            else {
                printf("Use HELP SNAPSHOT to open help menu.\n");
                free(input_copy);
                continue;
            }
        }
        if (!file_opened) {
            printf("Open database file first.\n");
            free(input_copy);
            continue;
        }
        else if (strcmp(token, "insert") == 0) {
            struct Record new_record = { 0 };
            insert(new_record, records, &records_size, token);
            free(input_copy);
            continue;
        }
        else if (strcmp(token, "query") == 0) {
            char* args = strtok(NULL, "");
            if (args != NULL) {
                while (*args == ' ') args++;
                query(records, records_size, args);
            }
            else {
                printf("Invalid command. Follow the format: QUERY ID=<ID>\n");
            }
            free(input_copy);
            continue;
        }

        else if (strcmp(token, "update") == 0) {
            char* args = strtok(NULL, "");
            if (args != NULL) {
                while (*args == ' ') args++;
                updateRecord(records, records_size, args);
            }
            else {
                printf("Invalid command. Follow the format: UPDATE ID=<id> <Field>=<Value>\n");
            }
            free(input_copy);
            continue;
        }

        else if (strcmp(token, "delete") == 0) {
            char* args = strtok(NULL, "");
            if (args != NULL) {
                while (*args == ' ') args++;
                delete_record(records, &records_size, args);
            }
            else {
                printf("Invalid command. Follow the format: DELETE ID=<ID>\n");
            }
            free(input_copy);
            continue;
        }

        else if (strcmp(token, "sort") == 0) {

            char* by = strtok(NULL, " ");
            char* field = strtok(NULL, " ");
            char* order = strtok(NULL, " ");

            if (!by || !field || !order || strcmp(by, "by") != 0) {
                printf("Please follow the format: SORT BY (ID / MARK) (ASC / DESC)\n");
                free(input_copy);
                continue;
            }

            sort_records(records, records_size, field, order);
            free(input_copy);
            continue;
        }

        else if (strcmp(token, "save") == 0) {
            save(records, records_size, current_filename);
            free(input_copy);
            continue;
        }

        else if (strcmp(token, "show all") == 0) {
            showall(records, records_size);
            free(input_copy);
            continue;
        }
        else if (strcmp(token, "show") == 0) {
            char *arg = strtok(NULL, " \n");
            if (arg != NULL && strcmp(arg, "all") == 0) {
                showall(records, records_size);
            }
            else if (arg != NULL && strcmp(arg, "summary") == 0) {
                showsummary(records, records_size);
            }
            else {
                printf("Unknown command. Use HELP to see help menu.\n");
            }
            free(input_copy);
            continue;
        }
        else if (strcmp(token, "showsummary") == 0) {
            showsummary(records, records_size);
            free(input_copy);
            continue;
        }
        printf("Unknown command. Use HELP to see help menu.\n");
        free(input_copy);


    } while (true);

    /* TO FREE MEMORY USED BY ARRAY
    for (size_t i = 0; i < total_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    */


    return 0;
}
