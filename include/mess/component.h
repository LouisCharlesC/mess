/**
 * @file component.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-04-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

namespace mess
{
	/**
	 * @brief Wraps a class to register it as a component to the broker.
	 * 
	 * @tparam ComponentType The class to wrap.
	 */
	template<typename ComponentType>
	class Component: public ComponentType
	{
	public:
		template<typename Broker>
		Component(Broker& broker):
			ComponentType(broker)
		{
			broker.component(static_cast<ComponentType&>(*this));
		}
	};
} // mess