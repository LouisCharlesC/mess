/**
 * @file mess.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include "provide.h"

#include <any>
#include <functional>
#include <type_traits>
#include <vector>

namespace mess
{
	template<enum Tags>
	struct TaggedValue
	{
		TaggedValue(Tags tag):
			m_tag(tag)
		{}

		Tags m_tag;
	};
	using Arguments = std::vector<NamedValue&>;
	using Invocable = std::function<NamedValue(Arguments)>;

}