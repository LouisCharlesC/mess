/**
 * @file broker.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-06-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "mess/mess.h"

/// You only need to forward declare endpoints here, you don't need to include their header!
class Logger;

using Broker = mess::Broker<Logger>;