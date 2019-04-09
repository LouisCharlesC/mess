/**
 * @file main.cpp
 * @author L.-C. C.
 * @brief Minimal mess example split in header and implementation files
 * @version 0.1
 * @date 2019-04-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "broker.h"
#include "console.h"
#include "interpreter.h"

#include "mess/mess.h"

int main(int argc, char **argv)
{
	// Instantiate broker
	Broker broker;

	// Instantiate components
	mess::Component<Console> console(broker);
	mess::Component<Interpreter> interpreter(broker);

	// Loop until user asks to quit
	console.loop();

	// Rejoice
	return EXIT_SUCCESS;
}