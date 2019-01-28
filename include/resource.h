/*
 * resource.h
 *
 *  Created on: Oct 11, 2018
 *      Author: lcc
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include "lockonce.hpp"
#include "pushlist.h"
#include "safe.hpp"
#include "spinmutex.hpp"

#include <algorithm>
#include <chrono>
#include <mutex>

namespace mess {
	template<typename ResourceType>
	class Resource
	{
		using SafeResource = safe::Safe<ResourceType, SpinMutex>;

	public:
		using Handle = typename SafeResource::template Access<LockOnce>;

		Handle get()
		{
			auto entry = std::find_if(m_resources.begin(), m_resources.end(), [](SafeResource& safeResource){return safeResource.lockable().try_lock();});
			if (entry == m_resources.endSentry())
			{
				entry = m_resources.push(std::try_to_lock);
			}
			return Handle(entry->unsafe(), entry->lockable(), std::adopt_lock);
		}

		template<typename... Args>
		void push(Args&&... args)
		{
			m_resources.push(safe::default_construct_lockable, std::forward<Args>(args)...);
		}

	private:
		PushList<safe::Safe<ResourceType, SpinMutex>> m_resources;
	};
}	// namespace mess

#endif /* RESOURCE_H_ */
