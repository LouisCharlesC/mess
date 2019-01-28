/**
 * @file lockonce.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-01-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <mutex>

namespace mess
{
	template<typename LockableType>
	class LockOnce: private std::unique_lock<LockableType>
	{
		/// Inherit all constructors from std::unique_lock and delete all
		/// those that would cause the lockable object not to be locked.
		/// LockOnce always has the lockable object locked!
		using std::unique_lock<LockableType>::unique_lock;
		LockOnce(LockableType& m, std::try_to_lock_t tag) = delete;
		LockOnce(LockableType& m, std::defer_lock_t tag) = delete;
		template <class Rep, class Period>
		LockOnce(LockableType& m, const std::chrono::duration<Rep,Period>& rel_time) = delete;
		template <class Clock, class Duration>
		LockOnce(LockableType& m, const std::chrono::time_point<Clock,Duration>& abs_time) = delete;

		using std::unique_lock<LockableType>::operator=;
		using std::unique_lock<LockableType>::swap;

		// Only const access to the lockable object
		const LockableType* mutex() const noexcept {return std::unique_lock<LockableType>::mutex();}
	};
} // namespace mess