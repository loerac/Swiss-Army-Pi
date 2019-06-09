#ifndef _TYPE_COMPAT_H_
#define _TYPE_COMPAT_H_

#include <stdlib.h>
#include <string.h>

#define istrcpy   (void)strcpy
#define istrncpy  (void)strncpy
#define isprintf  (void)sprintf
#define isnprintf (void)snprintf

#define imemset   (void)memset
#define imemcpy   (void)memcpy
#define imemmove  (void)memmove

#endif /* _TYPE_COMPAT_H_ */

