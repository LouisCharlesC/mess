/**
 * @file logger.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-06-08
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "log.h"

#include <iosfwd>

class Logger
{
public:
	using Component = std::ostream;

	template<typename Broker>
	static void onPublish(Log, Broker& broker, Component& logger, const char msg[]);
};