/**
 * @file messtraits.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-25
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

namespace mess
{
	template<typename Topic>
	struct TopicTraits
	{
		// using Message = ; // for publications and caches
		// static constexpr bool IsCacheEnabled = ; // for publications and caches
		// static const Message InitialValue = ; // for caches
		// using Provider = ; // for services
		// using Return = ; // for services
	};
	// const Message<topic_name> TopicTraits<topic_name>::InitialValue; // for caches

	template<typename Topic>
	using Message = typename TopicTraits<Topic>::Message;
	template<typename Service>
	using Return = typename TopicTraits<Service>::Return;
	template<typename Service>
	using Provider = typename TopicTraits<Service>::Provider;
} // namespace mess