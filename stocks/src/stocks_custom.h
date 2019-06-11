#ifndef _STOCKS_CUSTOM_H_
#define _STOCKS_CUSTOM_H_

#include "slist.h"
#include "stocks_common.h"

#include <stdbool.h>

/* Stock API parameter info */
#define MAX_URL_API_LEN       34U
#define MAX_URL_KEY_LEN       30U
#define MAX_TICKER_SYMBOL_LEN 5U

/* Max stock markets to monitor */
// TODO: For now, only hold 5 equities until
// able to wait 5 minutes (time library???)
//#define MAX_STOCK_EQUITIES    10U
#define MAX_STOCK_EQUITIES    5U

typedef struct stocks_operation {
   int time_interval;
   char url[MAX_URL_API_LEN + 1U];
   char key[MAX_URL_KEY_LEN + 1U];
} stocks_operation_s;

typedef struct time_series {
   time_series_function_e function;
   time_series_interval_e interval;
   bool is_compact;
   char symbol[MAX_TICKER_SYMBOL_LEN + 1U];
} time_series_s;

/**
 * INPUT:
 *    NONE
 *
 * OUTPUT:
 *    NONE
 *
 * RETURN:
 *    True if the JSON file is present and valid.
 *    Else, false.
 *
 * DESCRIPTION:
 *    Parses the customized stock exchange file.
 **/
bool stock_custom( void );

/**
 * INPUT:
 *    NONE
 *
 * OUTPUT:
 *    NONE
 *
 * RETURN:
 *    True if the JSON file is present and valid.
 *    Else, false.
 *
 * DESCRIPTION:
 *    Parses the URL API stock exchange.
 **/
bool stock_api_custom( void );

/**
 * RETURN:
 *    slist_s* of the stock equitys
 * DESCRIPTION:
 *    See RETURN for description
 **/
slist_s *getStockEquity( void );

/**
 * RETURN:
 *    stocks_operation_s of the API information
 * DESCRIPTION:
 *    See RETURN for description
 **/
stocks_operation_s getAPIOperation( void );

#endif /* _STOCKS_CUSTOM_H_ */

