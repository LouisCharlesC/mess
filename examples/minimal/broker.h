/**
 * @file broker.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "mess/brokertemplate.h"

// Declare components
class Console;
class Interpreter;

// Alias for the broker used in this program
using Broker = mess::BrokerTemplate<Console, Interpreter>;