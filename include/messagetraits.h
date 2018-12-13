/*
 * messagetraits.h
 *
 *  Created on: Nov. 11, 2018
 *      Author: lcc
 */

#ifndef MESSAGETRAITS_H_
#define MESSAGETRAITS_H_

#include <cstdint>

namespace mess {
	template<std::size_t Topic>
	struct MessageTraits
	{
		using Payload = void;
	};

	template<std::size_t Topic>
	using PayloadType = typename MessageTraits<Topic>::Payload;
}  // namespace mess

#endif /* MESSAGETRAITS_H_ */
