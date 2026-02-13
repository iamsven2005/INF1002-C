#ifndef HEADERS_H
#define HEADERS_H

#if defined(_WIN32) || defined(_WIN64)
    #include "dirent.h"   // third-party dirent
    #include <direct.h>   // _getcwd(), _chdir()
    #define getcwd _getcwd
#else
    #include <dirent.h>   // native POSIX dirent
    #include <unistd.h>   // getcwd(), chdir()
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define DATE_COMPLETED "24-11-2025"
#define FILENAME "TODO.txt"
#define USER "TODO"
#define CMS "CMS"
#define SHOW_ALL 1

// Custom Functions/Header files
#include "open.h"
#include "misc.h"
#include "insert.h"
#include "query.h"
#include "update.h"
#include "delete.h"
#include "save.h"
#include "showall.h"
#include "sort.h"
#include "showsummary.h"
#include "snapshot.h"

#endif
