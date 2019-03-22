/**
 * @file broker.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "brokertemplate.hpp"

class Printer;
class Sensor;

using Broker = mess::BrokerTemplate<Printer, Sensor>;