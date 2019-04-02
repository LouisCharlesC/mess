
#include "messtraits.h"

class Printer;

struct Print {};

namespace mess
{
	template<> struct TopicTraits<Print>
	{
		using Provider = Printer;
		using Return = void;
	};
	template<> Return<Print> onCall<Print>(Provider<Print>&);
} // mess