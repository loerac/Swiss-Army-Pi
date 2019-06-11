#ifndef _STOCKS_FTP_H_
#define _STOCKS_FTP_H_

#include "stocks.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct stock_info {
   size_t size;
   bool valid;
   char *data;
} stock_info_s;

bool stockEquityURL( void );
void destroyEquity( void );

#endif /* _STOCKS_FTP_H_ */

