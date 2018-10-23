/*
 * message.h
 *
 *  Created on: Sep 11, 2018
 *      Author: lcc
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <memory>

namespace mess {
	template<typename PayloadType>
	class Message;

	class AnyMessage
	{
	public:
//		template<typename PayloadType>
//		operator const Message<PayloadType>&() const {return }

		template<typename PayloadType>
		const PayloadType& get() const {return static_cast<const Message<PayloadType>&>(*this).get();}
		template<typename PayloadType>
		PayloadType& get() {return static_cast<const Message<PayloadType>&>(*this).get();}
	};

	template<typename PayloadType>
	class Message: public AnyMessage
	{
	public:
		template<typename... Args>
		Message(Args&&... args):
			m_payload(std::forward<Args>(args)...)
		{}

		const PayloadType& get() const {return m_payload;}

	private:
		PayloadType m_payload;
	};
}  // namespace mess

//#include "message.hpp"

#endif /* MESSAGE_H_ */
