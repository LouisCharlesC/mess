/**
 * @file ops.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

namespace mess
{
	enum class PullOperation
	{
		Read,
		Dereference,
		Invoke,
		Await
	};
} // namespace mess