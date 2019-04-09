/**
 * @file readme.cpp
 * @author L.-C. C.
 * @brief Minimal mess example in a single implementation file.
 * @version 0.1
 * @date 2019-04-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "mess/mess.h"

#include <cstddef>
#include <iostream>
#include <string>

// Declare components
class Console;
class Interpreter;

// Alias for the broker used in this program
using Broker = mess::BrokerTemplate<Console, Interpreter>;

// Declare topics
struct Interpret;
struct Quit;
// Define topic traits
namespace mess
{
	//											Topic								Topic again, Provider,	Return, Argument
	template<> struct Topic<Interpret>: Service<Interpret, Interpreter, void, const std::string&> {};
	//											Topic								Topic again
	template<> struct Topic<Quit>: Notification<Quit> {};
} // mess

// Define components
class Console
{
public:
	Console(Broker& broker):
		m_broker(broker)
	{}

	void quit()
	{
		m_quit = true;
	}

	void loop()
	{
		std::string input;
		while (!m_quit)
		{
			std::cin >> input;
			m_broker.call<Interpret>(input);
		}
	}

private:
	Broker& m_broker;
	bool m_quit = false;
};

class Interpreter
{
public:
	Interpreter(Broker& broker):
		m_broker(broker)
	{}

	void interpret(const std::string& command)
	{
		if (command == "q")
		{
			m_broker.notify<Quit>();
			std::cout << "Quit." << std::endl;
		}
		else
		{
			std::cout << "Echo: " << command << std::endl;
		}
	}

private:
	Broker& m_broker;
};

// Define callbacks
namespace mess
{
	template<> template<>
	void Topic<Quit>::Notification::onNotify(Console& subscriber)
	{
		subscriber.quit();
	}

	template<>
	void Topic<Interpret>::Service::onCall(Interpreter& provider, const std::string& command)
	{
		provider.interpret(command);
	}
} // mess

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