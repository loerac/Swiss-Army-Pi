#include "stocks_common.h"

#include <string.h>

/* See stocks_common.h for description */
ts_function_e tsFunction_stoi(const char *const function, const size_t function_len) {
   ts_function_e retval = MAX_TIME_SERIES_FUNCTION;
   if (0 == strncmp(function, "TIME_SERIES_INTRADAY", function_len)) {
      retval = TS_INTRADAY;
   } else if (0 == strncmp(function, "TIME_SERIES_DAILY", function_len)) {
      retval = TS_DAILY;
   } else if (0 == strncmp(function, "TIME_SERIES_DAILY_ADJUSTED", function_len)) {
      retval = TS_DAILY_ADJUSTED;
   } else if (0 == strncmp(function, "TIME_SERIES_WEEKLY", function_len)) {
      retval = TS_WEEKLY;
   } else if (0 == strncmp(function, "TIME_SERIES_WEEKLY_ADJUSTED", function_len)) {
      retval = TS_WEEKLY_ADJUSTED;
   } else if (0 == strncmp(function, "TIME_SERIES_MONTHLY", function_len)) {
      retval = TS_MONTHLY;
   } else if (0 == strncmp(function, "TIME_SERIES_MONTHLY_ADJUSTED", function_len)) {
      retval = TS_MONTHLY_ADJUSTED;
   } else if (0 == strncmp(function, "GLOBAL_QUOTE", function_len)) {
      retval = TS_GLOBAL_QUOTE;
   } else {
      // Intentional empty statement
   }

   return retval;
}

/* See stocks_common.h for description */
const char *tsFunction_itos(ts_function_e function) {
   const char *retval = NULL;
   switch (function) {
      case TS_INTRADAY :
         retval = "TIME_SERIES_INTRADAY ";
         break;
      case TS_DAILY:
         retval = "TIME_SERIES_DAILY";
         break;
      case TS_DAILY_ADJUSTED:
         retval = "TIME_SERIES_DAILY_ADJUSTED";
         break;
      case TS_WEEKLY:
         retval = "TIME_SERIES_WEEKLY";
         break;
      case TS_WEEKLY_ADJUSTED:
         retval = "TIME_SERIES_WEEKLY_ADJUSTED";
         break;
      case TS_MONTHLY:
         retval = "TIME_SERIES_MONTHLY";
         break;
      case TS_MONTHLY_ADJUSTED:
         retval = "TIME_SERIES_MONTHLY_ADJUSTED";
         break;
      case TS_GLOBAL_QUOTE:
         retval = "GLOBAL_QUOTE";
         break;
      default:
         retval = "ERROR";
         break;
   }

   return retval;
}

/* See stocks_common.h for description */
ts_interval_e tsInterval_stoi(const char *const interval, const size_t interval_len) {
   ts_interval_e retval = MAX_TIME_SERIES_INTERVAL;
   if (0 == strncmp(interval, "1min", interval_len)) {
      retval = TS_1_MINUTE;
   } else if (0 == strncmp(interval, "5min", interval_len)) {
      retval = TS_5_MINUTE;
   } else if (0 == strncmp(interval, "15min", interval_len)) {
      retval = TS_15_MINUTE;
   } else if (0 == strncmp(interval, "30min", interval_len)) {
      retval = TS_30_MINUTE;
   } else if (0 == strncmp(interval, "60min", interval_len)) {
      retval = TS_60_MINUTE;
   } else {
      // Intentional empty statement
   }

   return retval;
}

/* See stocks_common.h for description */
const char *tsInterval_itos(ts_interval_e interval) {
   const char *retval = NULL;
   switch (interval) {
      case TS_1_MINUTE:
         retval = "1min";
         break;
      case TS_5_MINUTE:
         retval = "5min";
         break;
      case TS_15_MINUTE:
         retval = "15min";
         break;
      case TS_30_MINUTE:
         retval = "30min";
         break;
      case TS_60_MINUTE:
         retval = "60min";
         break;
      default:
         retval = "ERROR";
         break;
   }

   return retval;
}

/* See stocks_common.h for description */
bool tsOpsize_stob(const char *const opsize, const size_t opsize_len) {
   return (0 == strncmp(opsize, "compact", opsize_len));
}

/* See stocks_common.h for description */
const char *tsOpsize_btos(const bool is_compact) {
   return (is_compact) ? "compact":"full";
}

/* See stocks_common.h for description */
const char *const getAPIURL( void ) {
   return "https://www.alphavantage.co/query?";
}
