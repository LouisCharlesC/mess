/**
 * @file tests.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-10-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <mess/mess.h>

#include <doctest/doctest.h>

#include <string>
#include <type_traits>

constexpr float aFunction() {return 40.f;}

constexpr unsigned int aOneArgumentFunction(unsigned int i) {return i-20u;}
constexpr float aTwoArgumentFunction(int i, float j) {return i+j;}

struct ANamedValue:
	mess::Names<42u> {};
struct AnotherNamedValue:
	mess::Names<41> {};

struct ANamedFunction:
	mess::Names<&aFunction> {};

struct ANamedInvokation:
	mess::Invokes<ANamedFunction>,
	mess::WithNoArgument {};

struct ANamedInvokationWithOneArgument:
	mess::Invokes<mess::Unnamed<&aOneArgumentFunction>>,
	mess::WithArguments<ANamedValue> {};

struct ANamedInvokationWithInvokableArguments:
	mess::Invokes<mess::Unnamed<&aTwoArgumentFunction>>,
	mess::WithArguments<AnotherNamedValue, ANamedInvokation> {};

TEST_CASE("")
{
	static_assert(std::is_same<decltype(mess::pull<ANamedValue>()), unsigned int>::value, "Bad pulled type!");
	static_assert(mess::pull<ANamedValue>() == 42, "Bad pulled value!");
	CHECK_EQ(mess::pull<ANamedValue>(), 42);

	static_assert(std::is_same<decltype(mess::pull<AnotherNamedValue>()), int>::value, "Bad pulled type!");
	static_assert(mess::pull<AnotherNamedValue>() == 41, "Bad pulled value!");
	CHECK_EQ(mess::pull<AnotherNamedValue>(), 41);

	static_assert(std::is_same<decltype(mess::pull<ANamedInvokation>()), float>::value, "Bad pulled type!");
	static_assert(mess::pull<ANamedInvokation>() == 40.f, "Bad pulled value!");
	CHECK_EQ(mess::pull<ANamedInvokation>(), 40.f);

	static_assert(std::is_same<decltype(mess::pull<ANamedInvokationWithOneArgument>()), unsigned int>::value, "Bad pulled type!");
	static_assert(mess::pull<ANamedInvokationWithOneArgument>() == 22, "Bad pulled value!");
	CHECK_EQ(mess::pull<ANamedInvokationWithOneArgument>(), 22);

	static_assert(std::is_same<decltype(mess::pull<ANamedInvokationWithInvokableArguments>()), float>::value, "Bad pulled type!");
	static_assert(mess::pull<ANamedInvokationWithInvokableArguments>() == 81.f, "Bad pulled value!");
	CHECK_EQ(mess::pull<ANamedInvokationWithInvokableArguments>(), 81.f);

	
	// REQUIRE(true);
	// CHECK(true);
}
