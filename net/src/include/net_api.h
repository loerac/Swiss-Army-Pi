#ifndef _NET_APIH_
#define _NET_APIH_

#include <stdbool.h>

#define MAX_DEV_LEN     32U
#define MAX_SSID_LENGTH 32U

typedef enum signal_strength {
   SIGNAL_STRENGTH_STRONG,
   SIGNAL_STRENGTH_GOOD,
   SIGNAL_STRENGTH_WEAK,
   SIGNAL_STRENGTH_BAD
} signal_strength_e;

typedef struct signal_info {
   int level;
   char ssid[MAX_SSID_LENGTH + 1U];
   signal_strength_e strength;
} signal_info_s;

bool netInit( void );
signal_info_s *getInternetInfo( void );
const bool internetAvail( void );

#endif /* _NET_API_H_ */
