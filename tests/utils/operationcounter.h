/**
 * @file operationcounter.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

class OperationCounter
{
public:
	OperationCounter() = default;
	OperationCounter(const OperationCounter& other); // copy constructor
	OperationCounter(OperationCounter&& other); // move constructor
	OperationCounter& operator=(const OperationCounter& other); // copy assignment
	OperationCounter& operator=(OperationCounter&& other); // move assignment
	
	bool checkCounts(
		unsigned int copyConstructCount,
		unsigned int moveConstructCount,
		unsigned int copyAssignCount,
		unsigned int moveAssignCount,
		bool movedFrom) const;

private:
	mutable unsigned int m_copyConstructCount = 0;
	unsigned int m_moveConstructCount = 0;
	mutable unsigned int m_copyAssignCount = 0;
	unsigned int m_moveAssignCount = 0;
	bool m_movedFrom = false;
};