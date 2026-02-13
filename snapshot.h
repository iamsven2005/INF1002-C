#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "headers.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define mkdir(path,mode) _mkdir(path)
#else
    #include <sys/stat.h>
#endif

bool file_in_dir(const char *path, const char *snapshot_name);

bool is_valid_snapshot_name(const char *snapshot_name);

bool construct_snapshot_dir(const char *cwd, char *buffer, size_t buffer_size);

bool create_snapshot(const char *snapshot_name, const char *file_path);

bool restore_snapshot(const char *snapshot_name, const char *file_path);

bool delete_snapshot(const char *snapshot_name, const char *file_path);

#endif // SNAPSHOT_H
