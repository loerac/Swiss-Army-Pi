#ifndef _STOCKS_CUSTOM_H_
#define _STOCKS_CUSTOM_H_

#include <stdbool.h>

/* Stock API parameter info */
#define MAX_DATATYPE_LEN      4U
#define MAX_TICKER_SYMBOL_LEN 5U
#define MAX_INTERVAL_LEN      5U
#define MAX_OUTPUT_SIZE_LEN   7U
#define MAX_TIME_SERIES_LEN   28U
#define MAX_URL_KEY_LEN       30U
#define MAX_URL_API_LEN       34U

#define MAX_OUTPUTSIZE_CALL_LEN  2U
#define MAX_INTERVAL_CALL_LEN    5U
#define MAX_FUNCTION_CALL_LEN    8U

const char *const valid_outputsize_calls[MAX_OUTPUTSIZE_CALL_LEN] = {
   "compact",
   "full"
};

const char *const valid_interval_calls[MAX_INTERVAL_CALL_LEN] = {
   "1min",
   "5min",
   "15min",
   "30min",
   "60min"
};

const char *const valid_function_calls[MAX_FUNCTION_CALL_LEN] = {
   "TIME_SERIES_INTRADAY",
   "TIME_SERIES_DAILY",
   "TIME_SERIES_DAILY_ADJUSTED",
   "TIME_SERIES_WEEKLY",
   "TIME_SERIES_WEEKLY_ADJUSTED",
   "TIME_SERIES_MONTH",
   "TIME_SERIES_MONTH_ADJUSTED",
   "GLOBAL_QUOTE"
};

/* Total amount of stock exchange to check */
/*
 * TODO: Blocked on linked-list library story. For right
 * now, we will only check on one stock exchange. There
 * will be another story to update this to use multiple
 * stock exchanges.
#define MAX_STOCK_EXCHANGE    5U
*/

typedef struct stocks_api_parameters {
   /* Required API parameters*/
   char function[MAX_TIME_SERIES_LEN + 1U];
   char symbol[MAX_TICKER_SYMBOL_LEN + 1U];
   char key[MAX_URL_KEY_LEN + 1U];
   char url[MAX_URL_API_LEN + 1U];

   /* Optional API parameters*/
   char interval[MAX_INTERVAL_LEN + 1U];
   char outputsize[MAX_OUTPUT_SIZE_LEN + 1U];
   char datatype[MAX_DATATYPE_LEN + 1U];
} stocks_api_parameters_s;

/**********************************************
 * INPUT:
 *    NONE

 * OUTPUT:
 *    NONE
 *
 * RETURN:
 *    True if the JSON file is present and valid.
 *    Else, false.
 *
 * DESCRIPTION:
 *    Parses the customized stock exchange file.
 **********************************************/
bool stock_custom( void );

/**********************************************
 * INPUT:
 *    NONE

 * OUTPUT:
 *    NONE
 *
 * RETURN:
 *    True if the JSON file is present and valid.
 *    Else, false.
 *
 * DESCRIPTION:
 *    Parses the URL API stock exchange.
 **********************************************/
bool stock_api_custom( void );

#endif /* _STOCKS_CUSTOM_H_ */

