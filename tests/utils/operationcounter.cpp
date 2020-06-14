/**
 * @file operationcounter.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "operationcounter.h"

#include <algorithm>

OperationCounter::OperationCounter(const OperationCounter& other):
	m_copyConstructCount(++other.m_copyConstructCount),
	m_moveConstructCount(other.m_moveConstructCount),
	m_copyAssignCount(other.m_copyAssignCount),
	m_moveAssignCount(other.m_moveAssignCount),
	m_movedFrom(other.m_movedFrom)
{}
OperationCounter::OperationCounter(OperationCounter&& other):
	m_copyConstructCount(other.m_copyConstructCount),
	m_moveConstructCount(++other.m_moveConstructCount),
	m_copyAssignCount(other.m_copyAssignCount),
	m_moveAssignCount(other.m_moveAssignCount),
	m_movedFrom(other.m_movedFrom)
{
	other.m_movedFrom = true;
}
OperationCounter& OperationCounter::operator=(const OperationCounter& other)
{
	m_copyConstructCount = other.m_copyConstructCount;
	m_moveConstructCount = other.m_moveConstructCount;
	m_copyAssignCount = 1 + std::max(m_copyAssignCount, other.m_copyAssignCount);
	other.m_copyAssignCount = m_copyAssignCount;
	m_moveAssignCount = other.m_moveAssignCount;
	m_movedFrom = other.m_movedFrom;
	return *this;
}
OperationCounter& OperationCounter::operator=(OperationCounter&& other)
{
	m_copyConstructCount = other.m_copyConstructCount;
	m_moveConstructCount = other.m_moveConstructCount;
	m_copyAssignCount = other.m_copyAssignCount;
	m_moveAssignCount = 1 + std::max(m_moveAssignCount, other.m_moveAssignCount);
	other.m_moveAssignCount = m_moveAssignCount;
	m_movedFrom = other.m_movedFrom;
	other.m_movedFrom = true;
	return *this;
}
	
bool OperationCounter::checkCounts(
	unsigned int copyConstructCount,
	unsigned int moveConstructCount,
	unsigned int copyAssignCount,
	unsigned int moveAssignCount,
	bool movedFrom) const
{
	return
		m_copyConstructCount==copyConstructCount &&
		m_moveConstructCount==moveConstructCount &&
		m_copyAssignCount==copyAssignCount &&
		m_moveAssignCount==moveAssignCount &&
		m_movedFrom==movedFrom;
}