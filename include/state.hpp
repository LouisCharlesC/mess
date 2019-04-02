/*
 * state.h
 *
 *  Created on: Oct. 23, 2018
 *      Author: lcc
 */

#pragma once

#include "safe/safe.h"

#include <memory>
#include <mutex>
#include <utility>

// TODO: see if there is a pattern, and instead of specializing, provide specific customization points like isUnique() and copy() ?

namespace mess {
	template<typename ValueType>
	class State
	{
		using SafeStateType = safe::Safe<ValueType>;

	public:
		/**
		 * @brief The result of calling get() on a State variable.
		 */
    using Handle = ValueType; 

		template<typename... Args>
		State(Args&&... args):
			m_safeState(safe::default_construct_lockable, std::forward<Args>(args)...)
		{}

		template<typename... Args>
		void set(Args&&... args)
		{
			*m_safeState.writeAccess() = ValueType(std::forward<Args>(args)...);
		}

		typename SafeStateType::template Access<> update()
		{
			return {m_safeState};
		}

		typename SafeStateType::template Access<std::lock_guard, safe::AccessMode::ReadOnly> access() const
		{
			return {m_safeState};
		}

		Handle get() const
		{
			return *m_safeState.readAccess();
		}

	private:
		SafeStateType m_safeState;
	};

	template<typename ValueType>
	class State<std::shared_ptr<ValueType>>
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
			auto&& state = m_safeState.writeAccess();

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

		typename SafeStateType::template Access<> update()
		{
			auto&& state = m_safeState.template writeAccess<std::unique_lock>();

			// If Handles on the State do exist
			if (!state->unique())
			{
				// Create a brand new copy of the state, it is now unique
				*state = std::make_shared<ValueType>(**state);
			}

			state.lock.release();
			// do not throw an exception between those two lines, else the mutex will remain locked!
			return {m_safeState, std::adopt_lock};
		}

		typename SafeStateType::template Access<std::lock_guard, safe::AccessMode::ReadOnly> access() const
		{
			return {m_safeState};
		}

		Handle get() const
		{
			return std::const_pointer_cast<const ValueType>(*m_safeState.readAccess());
		}

	private:
		SafeStateType m_safeState;
	};
}  // namespace mess
