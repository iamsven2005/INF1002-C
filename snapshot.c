#include "snapshot.h"

// PLACE SNAPSHOTS IN SEPARATE DIRECTORY
bool file_in_dir(const char *path, const char *snapshot_name) {
    // check if dir exists, creates dir if not
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) != 0) {
            perror("mkdir");
            return false;
        }
    }
    
    // open dir and search for file
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("Failed to open directory %s.\n", path);
        return false;
    }

    struct dirent *entry;
    bool found = false;
    
    while ((entry = readdir(dir)) != NULL) {
        if (snapshot_name == NULL) {
            // List all files (skip hidden files)
            if (entry->d_name[0] == '.') continue;
            printf("%s\n", entry->d_name);
        }
        else {
            // Search for specific file
            if (strcmp(entry->d_name, snapshot_name) == 0) {
                found = true;
                break;
            }
        }
    }
    
    closedir(dir);
    return found;
}

// Validate snapshot name 
bool is_valid_snapshot_name(const char *snapshot_name) {
    if (snapshot_name == NULL || strlen(snapshot_name) == 0) {
        printf("Invalid snapshot name.\n");
        return false;
    }

    // Check for path traversal attempts
    if (strstr(snapshot_name, "..") != NULL ||
        strchr(snapshot_name, '/') != NULL ||
        strchr(snapshot_name, '\\') != NULL) {
        printf("Invalid snapshot name. Cannot contain '..' or path separators.\n");
        return false;
    }
    
    // limit length to prevent buffer issues (declared max is 50 in main.c)
    if (strlen(snapshot_name) > 50) {
        printf("Snapshot name too long.\n");
        return false;
    }

    return true;
}

// Construct snapshot directory path: cwd/snapshots
bool construct_snapshot_dir(const char *cwd, char *buffer, size_t buffer_size) {
    int written = snprintf(buffer, buffer_size, "%s/snapshots", cwd);
    return (written >= 0 && (size_t)written < buffer_size);
}

// SNAPSHOT FUNCTION - now takes file_path parameter
bool create_snapshot(const char *snapshot_name, const char *file_path) {
    
    // open snapshot file
    FILE *snapshot_file = fopen(file_path, "r");
    if (snapshot_file != NULL) {
        printf("Snapshot \"%s\" already exists. Use a different name.\n", snapshot_name);
        fclose(snapshot_file);
        return false;
    }

    // Check if DB file exists
    FILE *db_file = fopen(FILENAME, "r");
    if (db_file == NULL) {
        printf("Unable to open database file \"%s\"\n", FILENAME);
        return false;
    }

    struct Record *records;
    int records_size = open_and_read_file(&records, FILENAME);

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\"\n", FILENAME);
        if (records_size > 0) free(records);
        return false;
    }

    if (records_size < 0) {
        printf("Unable to read from database file \"%s\"\n", FILENAME);
        return false;
    }

    snapshot_file = fopen(file_path, "w");
    if (snapshot_file == NULL) {
        printf("Unable to create snapshot file \"%s\"\n", snapshot_name);
        if (records_size > 0) free(records);
        return false;
    }

    for (int i = 0; i < records_size; i++) {
        fprintf(snapshot_file, "%d,%s,%s,%.2f\n", 
                records[i].id, records[i].name, records[i].prog, records[i].marks);
    }

    if (fclose(snapshot_file) != 0) {
        printf("Error closing snapshot file \"%s\"\n", snapshot_name);
        if (records_size > 0) free(records);
        return false;
    }

    printf("Snapshot created. Verifying...\n");

    struct Record *snapshot;
    int snapshot_size = open_and_read_file(&snapshot, file_path);

    if (snapshot_size != records_size) {
        printf("Snapshot verification failed. Snapshot \"%s\" size does not match current record \"%s\" size.\n", 
               snapshot_name, FILENAME);
        if (records_size > 0) free(records);
        if (snapshot_size > 0) free(snapshot);
        return false;
    }

    printf("Snapshot \"%s\" successfully created.\n", snapshot_name);

    if (records_size > 0) free(records);
    if (snapshot_size > 0) free(snapshot);

    return true;
}


// RESTORE FUNCTION
bool restore_snapshot(const char *snapshot_name, const char *file_path) {
    // Note: validation already done in main handler
    
    FILE *snapshot_file = fopen(file_path, "r");
    if (snapshot_file == NULL) {
        printf("Snapshot file \"%s\" does not exist.\n", snapshot_name);
        return false;
    }
    fclose(snapshot_file);

    struct Record *snapshot_records;
    int snapshot_size = open_and_read_file(&snapshot_records, file_path);

    if (snapshot_size < 0) {
        printf("Unable to read from snapshot file \"%s\".\n", snapshot_name);
        return false;
    }

    FILE *db_file = fopen(FILENAME, "w");
    if (db_file == NULL) {
        printf("Unable to open database file \"%s\" for writing.\n", FILENAME);
        if (snapshot_size > 0) free(snapshot_records);
        return false;
    }

    for (int i = 0; i < snapshot_size; i++) {
        fprintf(db_file, "%d,%s,%s,%.2f\n", 
                snapshot_records[i].id, snapshot_records[i].name, 
                snapshot_records[i].prog, snapshot_records[i].marks);
    }

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\" after restoring.\n", FILENAME);
        if (snapshot_size > 0) free(snapshot_records);
        return false;
    }
    
    printf("Snapshot restored. Verifying...\n");

    struct Record *restored_records;
    int restored_size = open_and_read_file(&restored_records, FILENAME);

    if (restored_size != snapshot_size) {
        printf("Restoration verification failed. Restored size does not match snapshot size.\n");
        if (snapshot_size > 0) free(snapshot_records);
        if (restored_size > 0) free(restored_records);
        return false;
    }

    printf("Snapshot \"%s\" successfully restored.\n", snapshot_name);

    if (snapshot_size > 0) free(snapshot_records);
    if (restored_size > 0) free(restored_records);

    return true;
}

bool delete_snapshot(const char *snapshot_name, const char *file_path) {
    // Note: validation already done in main handler
    
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Snapshot \"%s\" does not exist.\n", snapshot_name);
        return false;
    }
    fclose(file);

    if (remove(file_path) == 0) {
        printf("Snapshot \"%s\" successfully deleted.\n", snapshot_name);
        return true;
    } else {
        printf("Unable to delete snapshot \"%s\".\n", snapshot_name);
        return false;
    }
}
