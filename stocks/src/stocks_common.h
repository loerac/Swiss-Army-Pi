#ifndef _STOCKS_COMMON_H_
#define _STOCKS_COMMON_H_

#include <stdlib.h>
#include <stdbool.h>

typedef enum ts_function {
   TS_INTRADAY = 0,
   TS_DAILY,
   TS_DAILY_ADJUSTED,
   TS_WEEKLY,
   TS_WEEKLY_ADJUSTED,
   TS_MONTHLY,
   TS_MONTHLY_ADJUSTED,
   TS_GLOBAL_QUOTE,
   MAX_TIME_SERIES_FUNCTION
} ts_function_e;

typedef enum ts_interval {
   TS_1_MINUTE = 0,
   TS_5_MINUTE,
   TS_15_MINUTE,
   TS_30_MINUTE,
   TS_60_MINUTE,
   MAX_TIME_SERIES_INTERVAL
} ts_interval_e;

typedef enum ts_opsize {
   TS_COMPACT = 0,
   TS_FULL,
   MAX_TIME_SERIES_OPSIZE
} ts_opsize_e;

/**
 * INPUT:
 *    function - time series function in string format
 *    function_len - length of the string
 * RETURN:
 *    On success, the time series function value.
 *    On failure, invalid time series function value.
 * DESCRIPTION:
 *    Converts a time series function string format to
 *    a integer format
 **/
ts_function_e tsFunction_stoi(const char *const function, const size_t function_len);

/**
 * INPUT:
 *    function - time series function in integer format
 * RETURN:
 *    On success, the time series function string value.
 *    On failure, invalid time series function string value.
 * DESCRIPTION:
 *    Converts a time series function integer format to
 *    a string format
 **/
const char *tsFunction_itos(ts_function_e function);

/**
 * INPUT:
 *    interval - time series interval in string format
 *    interval_len - length of the string
 * RETURN:
 *    On success, the time series interval value.
 *    On failure, invalid time series interval value.
 * DESCRIPTION:
 *    Converts a time series interval string format to
 *    a integer format
 **/
ts_interval_e tsInterval_stoi(const char *const interval, const size_t interval_len);

/**
 * INPUT:
 *    interval - time series interval in integer format
 * RETURN:
 *    On success, the time series interval string value.
 *    On failure, invalid time series interval string value.
 * DESCRIPTION:
 *    Converts a time series interval integer format to
 *    a string format
 **/
const char *tsInterval_itos(ts_interval_e interval);

/**
 * INPUT:
 *    opsize - time series output size in string format
 *    opsize_len - length of the string
 * RETURN:
 *    On success, the time series output size value.
 *    On failure, invalid time series output size value.
 * DESCRIPTION:
 *    Converts a time series output size string format to
 *    a integer format
 **/
bool tsOpsize_stob(const char *const opsize, const size_t opsize_len);

/**
 * INPUT:
 *    is_compact - true if output size is "compact"
 * RETURN:
 *    On success, the time series output size string value.
 *    On failure, invalid time series output size string value.
 * DESCRIPTION:
 *    Converts the boolean "is_compact" value to string
 **/
const char *tsOpsize_btos(const bool is_compact);

/**
 * RETURN:
 *    Alpha Vantage API URL
 * DESCRIPTION:
 *    See RETURN for description
 **/
const char *const getAPIURL( void );

#endif /*_ STOCKS_COMMON_H_ */
