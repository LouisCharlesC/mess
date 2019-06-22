/**
 * @file logsub.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-06-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "mess/mess.h"

/// You only need to forward declare the channels and endpoints here, you don't need to include their header!
/// Forward declare Channel
class Log;
/// Forward declare Endpoint
class Logger;

namespace mess
{
	template<> struct Channel<Log>: Subscribers<Logger> {};
}