/**
 * @file stdcin.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <atomic>
#include <string>
#include <thread>

class Stdcin
{
public:
	~Stdcin();

	void quit();

private:
	void loop();
	void interpret(const std::string& input);

	std::atomic<bool> m_quit = ATOMIC_VAR_INIT(false);
	std::thread m_thread{&Stdcin::loop, this};
};