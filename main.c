#define _CRT_SECURE_NO_WARNINGS
#include "headers.h"

struct Record *records = NULL;
int records_size = 0;

int main(){


    /* === DECLARATION ===*/
    printf("Declaration\n");
    printf("%s",
        "Students are not allowed to copy assessment solutions from another person or other places, including\n"
        "AI platforms or tools. It is the students' responsibility to guarantee that their solutions are their\n"
        "own work. Meanwhile, the students must also ensure that their work is not accessible by others. Where\n"
        "such plagiarism is detected, both of the assessments involved will receive ZERO mark.\n\n"
        "We hereby declare that:\n"
        "- We fully understand and agree to the abovementioned policy.\n"
        "- We did not copy any materials from others or from other places, e.g., AI platforms or tools.\n"
        "- We did not share our materials with others or upload to any other places for public access.\n"
        "- We agree that we will not disclose any information or material of the team project to others or\n"
        "  upload to any other places for public access.\n"
        "- We agree that our project will receive Zero mark if any misalignment with the abovementioned\n"
        "  policies is detected.\n\n"
    );
    printf("Declared by: %s\n", USER);
    printf("%s",
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

    /*Prompts until the user enters a valid command within the input length limit*/
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

    /* Duplicate input so strtok can safely tokenize it without losing original command string */
        char *input_copy = strdup(input);

        if (input_copy == NULL) {
            printf("Input duplicate failed.\n");
            return 1;
        }

        char *command = strtok(input_copy, " ");
        if (command == NULL) {
            printf("Enter a command.\n");
            free(input_copy);
            continue;
        }

        if (strcasecmp_ci(command, "exit") == 0) {
            printf("Exiting...Goodbye :)\n");
            if (file_opened) {
                free(input_copy);
                free(records);
            }
            return 0;
        }
        else if (strcasecmp_ci(command, "help") == 0) {
            char *help_arg = strtok(NULL, "");
            if (help_arg != NULL) {
                while (*help_arg == ' ') help_arg++;
            }
            help_menu(help_arg);
            free(input_copy);
            continue;
        }
        else if (strcasecmp_ci(command, "open") == 0) {
            char *filename_arg = strtok(NULL, "");
            if (filename_arg != NULL) {
                while (*filename_arg == ' ') filename_arg++;
                if (*filename_arg != '\0') {
                    strncpy(current_filename, filename_arg, sizeof(current_filename) - 1);
                    current_filename[sizeof(current_filename) - 1] = '\0';
                }
            }

            if (file_opened && records != NULL) {
                free(records);
                records = NULL;
                records_size = 0;
            }

            /* call open file func */
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

        /* Handle snapshot commands*/
        if (find_substring_ci(input, "snapshot") != NULL) {
            char *snapshot_keyword = strtok(NULL, " \n");
            if (snapshot_keyword == NULL || strcasecmp_ci(snapshot_keyword, "snapshot") != 0){
                printf("Unknown command. Use HELP to see help menu.\n");
                free(input_copy);
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

            if ((snapshot_ptr = find_substring_ci(input, "show snapshot")) != NULL) {
                printf("\nAvailable snapshots:\n");
                file_in_dir(dir_path, NULL);
                free(input_copy);
                continue;
            }
            else if ((snapshot_ptr = find_substring_ci(input, "create snapshot")) != NULL) {
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

                create_snapshot(snapshot_name, file_path, current_filename);
                free(input_copy);
                continue;
            }
            else if ((snapshot_ptr = find_substring_ci(input, "restore snapshot")) != NULL) {
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
            else if ((snapshot_ptr = find_substring_ci(input, "delete snapshot")) != NULL) {
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

    /*Stops non-file operations until database has been opened successfully.*/
        if (!file_opened) {
            printf("Open database file first.\n");
            free(input_copy);
            continue;
        }
        else if (strcasecmp_ci(command, "insert") == 0) {
            struct Record new_record = { 0 };
            char *args = strtok(NULL, "");
            if (args != NULL) {
                while (*args == ' ') args++;
            }
            insert(new_record, records, &records_size, args);
            free(input_copy);
            continue;
        }
        else if (strcasecmp_ci(command, "query") == 0) {
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

        else if (strcasecmp_ci(command, "update") == 0) {
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

        else if (strcasecmp_ci(command, "delete") == 0) {
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

        else if (strcasecmp_ci(command, "sort") == 0) {

            char* by = strtok(NULL, " ");
            char* field = strtok(NULL, " ");
            char* order = strtok(NULL, " ");

            if (!by || !field || !order || strcasecmp_ci(by, "by") != 0) {
                printf("Please follow the format: SORT BY (ID / MARK) (ASC / DESC)\n");
                free(input_copy);
                continue;
            }

            sort_records(records, records_size, field, order);
            free(input_copy);
            continue;
        }

        else if (strcasecmp_ci(command, "save") == 0) {
            save(records, records_size, current_filename);
            free(input_copy);
            continue;
        }

        else if (strcasecmp_ci(command, "show all") == 0) {
            showall(records, records_size);
            free(input_copy);
            continue;
        }
        else if (strcasecmp_ci(command, "show") == 0) {
            char *show_arg = strtok(NULL, " \n");
            if (show_arg != NULL && strcasecmp_ci(show_arg, "all") == 0) {
                showall(records, records_size);
            }
            else if (show_arg != NULL && strcasecmp_ci(show_arg, "summary") == 0) {
                showsummary(records, records_size);
            }
            else {
                printf("Unknown command. Use HELP to see help menu.\n");
            }
            free(input_copy);
            continue;
        }
        else if (strcasecmp_ci(command, "showsummary") == 0) {
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
