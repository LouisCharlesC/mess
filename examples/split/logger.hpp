/**
 * @file logger.hpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-06-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "logger.h"

#include <ostream>

template<typename Broker>
void Logger::onPublish(Log, Broker& broker, Component& logger, const char msg[])
{
	logger << msg;
}