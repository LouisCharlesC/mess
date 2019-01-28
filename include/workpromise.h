/*
 * work.h
 *
 *  Created on: Oct 24, 2018
 *      Author: lcc
 */

#ifndef WORKPROMISE_H_
#define WORKPROMISE_H_

#include <functional>
#include <future>
#include <utility>

namespace mess {
  template<typename ReturnType>
  class WorkPromise
  {
  public:
    template<typename... Args>
    WorkPromise(Args&&... args):
      m_promise(),
      m_work(std::forward<Args>(args)...)
    {}

    template<typename T>
    friend void workPromiseCallTarget(WorkPromise<T>&);
    void operator()()
    {
    	workPromiseCallTarget(*this);
    }

    std::future<ReturnType> get_future()
    {
      return m_promise.get_future();
    }
  private:

    std::promise<ReturnType> m_promise;
    std::function<ReturnType()> m_work;
  };

	template<typename ReturnType>
	void workPromiseCallTarget(WorkPromise<ReturnType>& wp)
	{
		wp.m_promise.set_value(wp.m_work());
	}
	template<>
	void workPromiseCallTarget<void>(WorkPromise<void>& wp)
	{
		wp.m_work();
		wp.m_promise.set_value();
	}
}  // namespace mess

#endif /* WORKPROMISE_H_ */
