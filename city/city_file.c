#include "city.h"
#include "city_file.h"

static file_info file = {
    .in = NULL,
    .size = -1,
    .text = NULL,
    .name = {'\0'},
    .status = FILE_OK
};

bool file_destroy(file_info *f) {
    bool failed = true;
    if (fclose(f->in) != 0) {
        printf("Failed to close \'%s\': error(%s)\n", f->name, strerror(errno));
        f->status = FILE_CLOSE_ERROR;
        goto exit;
    }

    f->status = FILE_OK;
    free(f->text);
    f->size = -1;
    failed = false;

exit:
    return failed;
}

bool file_exist(file_info *f) {
    bool failed = true;
    if (access(f->name, F_OK) == -1) {
        printf("File \'%s\' not found: error(%s)\n", f->name, strerror(errno));
        f->status = FILE_NOT_FOUND;
        goto exit;
    }
    failed = false;

exit:
    return failed;
}

bool file_open(file_info *f) {
    bool failed = true;
    if (!(f->in = fopen(f->name, "rb"))) {
        printf("Failed to open \'%s\': error(%s)\n", f->name, strerror(errno));
        f->status = FILE_OPEN_ERROR;
        goto exit;
    }
    failed = false;

exit:
    return failed;
}

bool file_read(file_info *f) {
    bool failed = true;
    fseek(f->in, 0L, SEEK_END);
    f->size = ftell(f->in);
    rewind(f->in);
    f->text = malloc(f->size);
    if (fread(f->text, f->size, 1, f->in) != 1) {
        printf("Failed to read \'%s\': error(%s)\n", f->name, strerror(errno));
        file_status sts = file_destroy(f);
        f->status = (sts != FILE_OK) ? sts:FILE_READ_ERROR;
        goto exit;
    }
    failed = false;

exit:
    return failed;
}

bool file_process(const char *fname) {
    bool failed = true;
    if ((fname != NULL) && (fname[0] == '\0')) {
        printf("No file given\n");
        strncpy(file.name, "(null)", MAX_FILE_NAME_SIZE);
        file.status = FILE_NULL_PTR;
        goto process_exit;
    }

    strncpy(file.name, fname, MAX_FILE_NAME_SIZE);
    failed = file_exist(&file);
    if (failed) {
        goto process_exit;
    }

    failed = file_open(&file);
    if (failed) {
        goto process_exit;
    }

    failed = file_read(&file);
    if (failed) {
        goto process_exit;
    }

    if (file.status == FILE_OK) {
        goto exit;
    }

process_exit:
    printf("Processing \'%s\' failed: status(%d)\n", file.name, file.status);

exit:
    return failed;
}

const char *get_text( void ) {
    return file.text;
}

const file_status get_status( void ) {
    return file.status;
}
