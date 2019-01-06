#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <json-c/json.h>

#include "city.h"
#include "city_file.h"
#include "city_types.h"

static city_file city = { '\0' };

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
    (void)fseek(f->in, 0L, SEEK_END);
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

    (void)strncpy(city.name, json, sizeof(city.name));

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

void jsonUnitURL(url_sts *u) {
    switch (u->e_format) {
    case METRIC:
        (void)strncpy(u->s_format, "metric", MAX_UNIT_FORMAT);
        break;
    case KELVIN:
        (void)strncpy(u->s_format, "kelvin", MAX_UNIT_FORMAT);
        break;
    default:
        (void)strncpy(u->s_format, "imperial", MAX_UNIT_FORMAT);
    }
}

void customCoord(json_object *jb, url_sts *u, const char *k) {
    u->find_by |= CITY_COORD;
    json_object *new_obj = json_object_object_get(jb, k);
    json_object_object_foreach(new_obj, key, val) {
        if (strncmp(key, "lon", sizeof("lon")) == 0) {
            (void)strncpy(u->lon, json_object_get_string(val), MAX_LAT_LON);
        } else if (strncmp(key, "lat", sizeof("lat")) == 0) {
            (void)strncpy(u->lat, json_object_get_string(val), MAX_LAT_LON);
        }
    }
}

void customCity(json_object *jb, url_sts *u, const char *k) {
    json_object *new_object = json_object_object_get(jb, k);
    json_object_object_foreach(new_object, key, val) {
        if (strncmp(key, "name", sizeof("name")) == 0) {
            (void)strncpy(u->city, json_object_get_string(val), MAX_CITY_NAME);
            u->find_by |= CITY_NAME;
        } else if (strncmp(key, "zipcode", sizeof("zipcode")) == 0) {
            (void)strncpy(u->zip, json_object_get_string(val), MAX_ZIPCODE);
            u->find_by |= CITY_ZIP;
        }
    }
}

void customRegion(json_object *jb, url_sts *u, const char *k) {
    json_object *new_object = json_object_object_get(jb, k);
    json_object_object_foreach(new_object, key, val) {
        if (strncmp(key, "country", sizeof("country")) == 0) {
            (void)strncpy(u->country, json_object_get_string(val), MAX_COUNTRY_CODE);
        } else if (strncmp(key, "city", sizeof("city")) == 0) {
            customCity(new_object, u, key);
        }
    }
}

bool customParse(url_sts *u) {
    bool failed = true;
    if (    (city.size == -1)
        &&  (city.status != FILE_OK) ) {
        printf("Invalid file\n");
        goto exit;
    }
    u->e_format = 0;
    jsonUnitURL(u);

    json_object *obj = json_tokener_parse(city.text);
    json_object_object_foreach(obj, key, val) {
        if (strncmp(key, "id", sizeof("id")) == 0) {
            (void)strncpy(u->id, json_object_get_string(val), MAX_ID);
        } else if (strncmp(key, "key", sizeof("key")) == 0) {
            (void)strncpy(u->key, json_object_get_string(val), MAX_KEY_SIZE);
        } else if (strncmp(key, "language", sizeof("language")) == 0) {
            (void)strncpy(u->lang, json_object_get_string(val), MAX_LANG_SIZE);
        } else if (strncmp(key, "region", sizeof("region")) == 0) {
            customRegion(obj, u, key);
        } else if (strncmp(key, "coord", sizeof("coord")) == 0) {
            customCoord(obj, u, key);
        } else if (strncmp(key, "units", sizeof("units")) == 0) {
            u->e_format = json_object_get_int(val);
            jsonUnitURL(u);
        }
    }
    failed = false;

exit:
    return failed;
}
