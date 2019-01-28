/**
 * @file spinmutex.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-01-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <atomic>
#include <mutex>

namespace mess
{
	struct set_flag_t {};
	static constexpr set_flag_t set_flag;

	class SpinMutex
	{
	public:
		SpinMutex() = default;

		SpinMutex(std::try_to_lock_t)
		{
			m_flag.test_and_set();
		}

		void lock()
		{
			while (m_flag.test_and_set())
			{}
		}
		bool try_lock()
		{
			return !m_flag.test_and_set();
		}
		void unlock()
		{
			m_flag.clear();
		}
	private:
		std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
	};
} // namespace mess