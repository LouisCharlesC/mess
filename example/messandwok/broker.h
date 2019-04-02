/**
 * @file broker.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "brokertemplate.hpp"

class Camera;
class Processor;
// class Checker;
class Monitor;

using Broker = mess::BrokerTemplate<Camera, Processor, Monitor>;