#ifndef _CITY_FILEH_
#define _CITY_FILEH_

#include "city.h"
#include <unistd.h>

#define MAX_FILE_NAME_SIZE 15U

typedef enum file_status {
    FILE_OK = 0,
    FILE_OPEN_ERROR,
    FILE_CLOSE_ERROR,
    FILE_READ_ERROR,
    FILE_WRITE_ERROR,
    FILE_NOT_FOUND,
    FILE_NULL_PTR,
} file_status;

typedef struct file_info_s {
    FILE *in;
    long size;
    char *text;
    char name[MAX_FILE_NAME_SIZE];
    file_status status;
} file_info;

bool file_process(const char *fname);
const char *get_text();
const file_status get_status();

#endif
