#ifndef _STOCKS_PARSER_H_
#define _STOCKS_PARSER_H_

#include "ftp.h"

#include <stdbool.h>

bool stocksParser(ftp_info_s ftp);
bool stocksParserAdjusted(ftp_info_s ftp);

#endif /* _STOCKS_PARSER_H_ */

