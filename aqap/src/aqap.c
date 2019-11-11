#include "net_api.h"
#include "aqap_config.h"
#include "type_compat.h"

#include <stdio.h>
#include <stdbool.h>

typedef enum aqap_status {
    AQAP_OK = 0,
    AQAP_FAILURE,
    AQAP_EXTERNAL_FAILURE
} aqap_status_e;

/**
 * INPUT:
 *    NONE
 * OUTPUT:
 *    NONE
 * RETURN:
 *    AQAP_OK
 *       - Start-up is successful
 *    AQAP_FAILURE
 *       - Configuration/customiztion failed
 *    AQAP_EXTERNAL_FAILURE
 *       - External services failed
 * DESCRIPTION:
 **/
aqap_status_e aqap_init( void ) {
    if (!aqap_config_init()) {
        printf("WARNING: AQAP Configuration FAILED\n");
        return AQAP_FAILURE;
    }
    printf("NOTICE: AQAP Configuration VALIDATED\n");

    if (!aqap_custom_init()) {
        printf("WARNING: AQAP Customization FAILED\n");
        return AQAP_FAILURE;
    }
    printf("NOTICE: AQAP Customization VALIDATED\n");

    if ( (!netInit()) ||
         (!internetAvail()) ) {
        printf("WARNING: Internet access FAILED\n");
        return AQAP_EXTERNAL_FAILURE;
    }
    printf("NOTICE: Internet access CONNECTED");

    return AQAP_OK;
}

int main(int argc, char *argv[]) {
    aqap_status_e aqap_sts = AQAP_OK;

    aqap_sts = aqap_init();
    switch (aqap_sts) {
        case AQAP_OK:
            printf("NOTICE: AQAP start-up OK\n");
            break;
        case AQAP_FAILURE:
            printf("WARNING: AQAP start-up FAILED\n");
            break;
        case AQAP_EXTERNAL_FAILURE:
            printf("WARNING: AQAP external services FAILED\n");
            break;
    }

    if (AQAP_OK == aqap_sts) {
        // Build URL and get FTP data
    }

    return aqap_sts;
}

