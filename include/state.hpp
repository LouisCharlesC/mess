/*
 * state.h
 *
 *  Created on: Oct. 23, 2018
 *      Author: lcc
 */

#ifndef STATE_H_
#define STATE_H_

#include "safe.hpp"

#include <memory>
#include <mutex>
#include <utility>

namespace mess {
	template<typename ValueType>
	class State
	{
		using SafeStateType = safe::Safe<std::shared_ptr<ValueType>>;
	public:
		/**
		 * @brief The result of calling get() on a State variable. The Handle is a pointer
		 * to the value of the State variable at the moment you called get(), yet, the is
		 * guaranteed not to change during the course of the Handle's lifetime.
		 */
    using Handle = std::shared_ptr<const ValueType>;

		template<typename... Args>
		State(Args&&... args):
			m_safeState(safe::default_construct_lockable, std::make_shared<ValueType>(std::forward<Args>(args)...))
		{}

		template<typename... Args>
		void set(Args&&... args)
		{
			safe::LockGuard<SafeStateType> state(m_safeState);

			// If Handles on the State do exist
			if (!state->unique())
			{
				// Construct a new shared_pointer
				*state = std::make_shared<ValueType>(std::forward<Args>(args)...);
			}
			else // no one owns a Handle on the State
			{
				// replace the contents of the shared_ptr
				**state = ValueType(std::forward<Args>(args)...);
			}
		}

		safe::LockGuard<SafeStateType> update()
		{
			safe::UniqueLock<SafeStateType> state(m_safeState);

			// If Handles on the State do exist
			if (!state->unique())
			{
				// Create a brand new copy of the state, it is now unique
				*state = std::make_shared<ValueType>(**state);
			}

			// return a safe guard of the State's value
			return {*state, *state.lock.release(), std::adopt_lock};
		}

		Handle get() const
		{
			return std::const_pointer_cast<const ValueType>(*safe::LockGuard<SafeStateType, safe::ReadOnly>(m_safeState));
		}

	private:
		SafeStateType m_safeState;
	};
}  // namespace mess

#endif /* STATE_H_ */
