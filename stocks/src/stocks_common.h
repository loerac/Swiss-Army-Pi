#ifndef _STOCKS_COMMON_H_
#define _STOCKS_COMMON_H_

typedef enum time_series_function {
   TIME_SERIES_INTRADAY = 0,
   TIME_SERIES_DAILY,
   TIME_SERIES_DAILY_ADJUSTED,
   TIME_SERIES_WEEKLY,
   TIME_SERIES_WEEKLY_ADJUSTED,
   TIME_SERIES_MONTH,
   TIME_SERIES_MONTH_ADJUSTED,
   TIME_SERIES_GLOBAL_QUOTE,
   MAX_TIME_SERIES_FUNCTION
} time_series_function_e;

typedef enum time_series_interval {
   TIME_SERIES_1_MINUTE = 0,
   TIME_SERIES_5_MINUTE,
   TIME_SERIES_15_MINUTE,
   TIME_SERIES_30_MINUTE,
   TIME_SERIES_60_MINUTE,
   MAX_TIME_SERIES_INTERVAL
} time_series_interval_e;

typedef enum time_series_opsize {
   TIME_SERIES_COMPACT = 0,
   TIME_SERIES_FULL,
   MAX_TIME_SERIES_OPSIZE
} time_series_opsize_e;

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
time_series_function_e tsFunctionInt(const char *const function, const size_t function_len);

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
const char *tsFunctionString(time_series_function_e function);

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
time_series_interval_e tsIntervalInt(const char *const interval, const size_t interval_len);

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
const char *tsIntervalString(time_series_interval_e interval);

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
time_series_opsize_e tsOpsizeString(const char *const opsize, const size_t opsize_len);

/**
 * INPUT:
 *    opsize - time series output size in integer format
 * RETURN:
 *    On success, the time series output size string value.
 *    On failure, invalid time series output size string value.
 * DESCRIPTION:
 *    Converts a time series output size integer format to
 *    a string format
 **/
const char *tsOpsizeString(time_series_opsize_e opsize);

#endif /*_ STOCKS_COMMON_H_ */
