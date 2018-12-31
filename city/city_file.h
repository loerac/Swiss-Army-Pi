#ifndef _CITY_FILEH_
#define _CITY_FILEH_

#include <unistd.h>
#include <stdbool.h>
#include <linux/limits.h>

typedef enum file_status {
    FILE_OK = 0,
    FILE_OPEN_ERROR,
    FILE_CLOSE_ERROR,
    FILE_READ_ERROR,
    FILE_WRITE_ERROR,
    FILE_NOT_FOUND,
    FILE_NULL_PTR,
} file_status;

typedef struct city_file_s {
    FILE *in;
    long size;
    char *text;
    char name[PATH_MAX + 1];
    file_status status;
} city_file;

bool fileProcess(const char *json);
bool parseCustom(url_sts *u);

#endif
