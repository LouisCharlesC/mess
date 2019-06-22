/**
 * @file logger.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-06-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "broker.h"
#include "logger.hpp"

// This is the explicite instantiation of the callback, if you wish to pre-compile that
template void Logger::onPublish<Broker>(Log, Broker&, Component&, const char[]);