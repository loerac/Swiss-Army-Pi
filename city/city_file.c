#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "city.h"
#include "city_file.h"

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE);  } while(0);

static city_file city = {
    .in = NULL,
    .size = -1,
    .text = "",
    .name = {'\0'},
    .status = FILE_OK
};

bool validateJSON(const char *json, const char *schema) {
    pid_t pid;
    int status;
    int link[2];
    bool failed = true;
    char *arg[5] = {'\0'};
    char valid[4096] = {'\0'};
    
    if (pipe(link) == -1) {
        die("pipe");
    } else if ((pid = fork()) == -1) {
        die("fork");
        printf("Forking error: error(%s)\n", strerror(errno));
    } else if (pid == 0) {
        dup2(link[1], STDERR_FILENO);
        close(link[0]);
        close(link[1]);
        arg[0] = "/usr/bin/json";
        arg[1] = "validate";
        // FIXME: Values not being written
        snprintf(arg[2], 64, "--schema-file=%s", schema);
        snprintf(arg[3], 64, "--document-file=%s", json);
        printf("arg[2]-----------%s\n", arg[2]);
        printf("arg[3]-----------%s\n", arg[3]);
        status = execv(arg[0], arg);
        if (status == -1) {
            printf("Error while validating JSON: error(%s)\n", strerror(errno));
        }
        die("execl");
    } else {
        /*if ((pid = waitpid(pid, &status, WNOHANG)) == -1) {
            printf("Failed waitpid: error(%s)\n", strerror(errno));
        } else {
            do {
                if (WIFEXITED(status)) {
                    printf("Child exited with status of %d\n", WEXITSTATUS(status));
                } else if (pid == 0) {
                    sleep(1);
                } else {
                    printf("Child did not exit successfully\n");
                }
            } while (pid == 0);
        }*/
        close(link[1]);
        int nbytes = read(link[0], valid, sizeof(valid));
        printf("%s\n", valid);
        if (nbytes == -1) {
            printf("Error while reading STDERR_FILENO: error(%s)\n", strerror(errno));
        } else if (valid[0] == '\0') {
            failed = false;
        } else {
            printf("%s failed validation\n", json);
        }
    }

    return failed;
}

bool fileClose(city_file *f) {
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

bool fileExist(city_file *f) {
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

bool fileOpen(city_file *f) {
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

bool fileRead(city_file *f) {
    bool failed = true;
    fseek(f->in, 0L, SEEK_END);
    f->size = ftell(f->in);
    rewind(f->in);
    f->text = malloc(f->size);
    if (fread(f->text, f->size, 1, f->in) != 1) {
        printf("Failed to read \'%s\': error(%s)\n", f->name, strerror(errno));
        file_status sts = fileClose(f);
        f->status = (sts != FILE_OK) ? sts:FILE_READ_ERROR;
        goto exit;
    }
    failed = false;

exit:
    return failed;
}

bool fileProcess(const char *json, const char *schema) {
    bool failed = true;
    if ((json != NULL) && (json[0] == '\0')) {
        printf("No file given\n");
        city.status = FILE_NULL_PTR;
        goto process_exit;
    }

    strncpy(city.name, json, sizeof(city.name));   
    failed = validateJSON(city.name, schema);
    if (failed) {
        goto process_exit;
    }

    failed = fileExist(&city);
    if (failed) {
        goto process_exit;
    }

    failed = fileOpen(&city);
    if (failed) {
        goto process_exit;
    }

    failed = fileRead(&city);
    if (failed) {
        goto process_exit;
    }

    if (city.status == FILE_OK) {
        goto exit;
    }

process_exit:
    printf("Processing \'%s\' failed: status(%d)\n", city.name, city.status);

exit:
    return failed;
}

const city_file getCityFile( void ) {
    return city;
}
