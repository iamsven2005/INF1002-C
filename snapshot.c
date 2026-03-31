#include "snapshot.h"

// Build metadata gile path for a snapshot by appending .meta to the snapshot file path 
static bool construct_snapshot_meta_path(const char *snapshot_path, char *buffer, size_t buffer_size) {
    int written = snprintf(buffer, buffer_size, "%s.meta", snapshot_path);
    return (written >= 0 && (size_t)written < buffer_size);
}

// Ensure snapshots directory exist,eihter list all snapshot or check if a named snapshot exists
bool file_in_dir(const char *path, const char *snapshot_name) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) != 0) {
            perror("mkdir");
            return false;
        }
    }
    

    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("Failed to open directory %s.\n", path);
        return false;
    }

    struct dirent *entry;
    bool found = false;
    
    while ((entry = readdir(dir)) != NULL) {
        size_t entry_len = strlen(entry->d_name);
//Skip hidden and metadata files so only real snapshot names are shown
        if (entry->d_name[0] == '.') continue;
        if (entry_len > 5 && strcmp(entry->d_name + entry_len - 5, ".meta") == 0) continue;

        if (snapshot_name == NULL) {
            printf("%s\n", entry->d_name);
        }
        else {
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

// Creates a snapshot of selected database file and store its source filename in a metadata file.
bool create_snapshot(const char *snapshot_name, const char *file_path, const char *db_filename) {
    // Build the matching metadata file path for this snapshot
    char meta_path[PATH_MAX] = "";
    if (!construct_snapshot_meta_path(file_path, meta_path, sizeof(meta_path))) {
        printf("Error: Metadata path too long.\n");
        return false;
    }
// Prevents duplicate snapshot
    FILE *snapshot_file = fopen(file_path, "r");
    if (snapshot_file != NULL) {
        printf("Snapshot \"%s\" already exists. Use a different name.\n", snapshot_name);
        fclose(snapshot_file);
        return false;
    }

    FILE *meta_file = fopen(meta_path, "r");
    if (meta_file != NULL) {
        printf("Snapshot \"%s\" already exists. Use a different name.\n", snapshot_name);
        fclose(meta_file);
        return false;
    }
// Open and load the soruce database file that is currently being backed up
    FILE *db_file = fopen(db_filename, "r");
    if (db_file == NULL) {
        printf("Unable to open database file \"%s\"\n", db_filename);
        return false;
    }

    struct Record *records = NULL;
    int records_size = execute_on_file(&records, db_filename);

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\"\n", db_filename);
        if (records != NULL) free(records);
        return false;
    }

    if (records_size < 0) {
        printf("Unable to read from database file \"%s\"\n", db_filename);
        if (records != NULL) free(records);
        return false;
    }
// Writing the snapshot data file using the same format
    snapshot_file = fopen(file_path, "w");
    if (snapshot_file == NULL) {
        printf("Unable to create snapshot file \"%s\"\n", snapshot_name);
        if (records != NULL) free(records);
        return false;
    }

    for (int i = 0; i < records_size; i++) {
        fprintf(snapshot_file, "%d,%s,%s,%.1f\n",
                records[i].id, records[i].name, records[i].prog, records[i].marks);
    }

    if (fclose(snapshot_file) != 0) {
        printf("Error closing snapshot file \"%s\"\n", snapshot_name);
        if (records != NULL) free(records);
        return false;
    }
// Save the origianl database file in a separate metadata file so snapshot can restore to the correct file
    meta_file = fopen(meta_path, "w");
    if (meta_file == NULL) {
        printf("Unable to create snapshot metadata file \"%s.meta\"\n", snapshot_name);
        remove(file_path);
        if (records != NULL) free(records);
        return false;
    }

    fprintf(meta_file, "%s\n", db_filename);

    if (fclose(meta_file) != 0) {
        printf("Error closing snapshot metadata file \"%s.meta\"\n", snapshot_name);
        remove(file_path);
        remove(meta_path);
        if (records != NULL) free(records);
        return false;
    }

    printf("Snapshot created. Verifying...\n");

    struct Record *snapshot_records = NULL;
    int snapshot_size = execute_on_file(&snapshot_records, file_path);

    if (snapshot_size != records_size) {
        printf("Snapshot verification failed. Snapshot \"%s\" size does not match current record \"%s\" size.\n",
               snapshot_name, db_filename);
        if (records != NULL) free(records);
        if (snapshot_records != NULL) free(snapshot_records);
        return false;
    }

    printf("Snapshot \"%s\" successfully created for database \"%s\".\n", snapshot_name, db_filename);

    if (records != NULL) free(records);
    if (snapshot_records != NULL) free(snapshot_records);

    return true;
}
// Resotre snapshot by reading its metadata file, finding original database filename, and rewriting
bool restore_snapshot(const char *snapshot_name, const char *file_path, char *restored_filename, size_t restored_filename_size) {
    char meta_path[PATH_MAX] = "";
    // Metadata filepath so the original database filename can be restored
    if (!construct_snapshot_meta_path(file_path, meta_path, sizeof(meta_path))) {
        printf("Error: Metadata path too long.\n");
        return false;
    }

    FILE *snapshot_file = fopen(file_path, "r");
    if (snapshot_file == NULL) {
        printf("Snapshot file \"%s\" does not exist.\n", snapshot_name);
        return false;
    }
    fclose(snapshot_file);
    // Read the original database filename from the metadata file
    FILE *meta_file = fopen(meta_path, "r");
    if (meta_file == NULL) {
        printf("Snapshot metadata for \"%s\" does not exist.\n", snapshot_name);
        return false;
    }

    char db_filename[PATH_MAX] = "";
    if (fgets(db_filename, sizeof(db_filename), meta_file) == NULL) {
        printf("Snapshot metadata for \"%s\" is empty.\n", snapshot_name);
        fclose(meta_file);
        return false;
    }
    fclose(meta_file);

    db_filename[strcspn(db_filename, "\r\n")] = '\0';

    if (strlen(db_filename) == 0) {
        printf("Snapshot metadata for \"%s\" is invalid.\n", snapshot_name);
        return false;
    }
    // Load all record from the snapshot data file into memory before rewritting
    struct Record *snapshot_records = NULL;
    int snapshot_size = execute_on_file(&snapshot_records, file_path);

    if (snapshot_size < 0) {
        printf("Unable to read from snapshot file \"%s\".\n", snapshot_name);
        return false;
    }
    // Overwrite the origianl database file with the records stored in the snapshot
    FILE *db_file = fopen(db_filename, "w");
    if (db_file == NULL) {
        printf("Unable to open database file \"%s\" for writing.\n", db_filename);
        if (snapshot_records != NULL) free(snapshot_records);
        return false;
    }

    for (int i = 0; i < snapshot_size; i++) {
        fprintf(db_file, "%d,%s,%s,%.1f\n",
                snapshot_records[i].id, snapshot_records[i].name,
                snapshot_records[i].prog, snapshot_records[i].marks);
    }

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\" after restoring.\n", db_filename);
        if (snapshot_records != NULL) free(snapshot_records);
        return false;
    }

    printf("Snapshot restored. Verifying...\n");

    struct Record *restored_records = NULL;
    int restored_size = execute_on_file(&restored_records, db_filename);

    if (restored_size != snapshot_size) {
        printf("Restoration verification failed. Restored size does not match snapshot size.\n");
        if (snapshot_records != NULL) free(snapshot_records);
        if (restored_records != NULL) free(restored_records);
        return false;
    }

    if (restored_filename != NULL && restored_filename_size > 0) {
        strncpy(restored_filename, db_filename, restored_filename_size - 1);
        restored_filename[restored_filename_size - 1] = '\0';
    }

    printf("Snapshot \"%s\" successfully restored to database \"%s\".\n", snapshot_name, db_filename);

    if (snapshot_records != NULL) free(snapshot_records);
    if (restored_records != NULL) free(restored_records);

    return true;
}
// Delete both the snapshot data file and its matching metadata file
bool delete_snapshot(const char *snapshot_name, const char *file_path) {
    char meta_path[PATH_MAX] = "";
    if (!construct_snapshot_meta_path(file_path, meta_path, sizeof(meta_path))) {
        printf("Error: Metadata path too long.\n");
        return false;
    }

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Snapshot \"%s\" does not exist.\n", snapshot_name);
        return false;
    }
    fclose(file);

    if (remove(file_path) != 0) {
        printf("Unable to delete snapshot \"%s\".\n", snapshot_name);
        return false;
    }

    FILE *meta_file = fopen(meta_path, "r");
    if (meta_file != NULL) {
        fclose(meta_file);
        if (remove(meta_path) != 0) {
            printf("Snapshot \"%s\" data file was deleted, but its metadata file could not be removed.\n", snapshot_name);
            return false;
        }
    }

    printf("Snapshot \"%s\" successfully deleted.\n", snapshot_name);
    return true;
}
