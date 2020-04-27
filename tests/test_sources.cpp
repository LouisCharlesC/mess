/**
 * @file test_mess.cpp
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2020-04-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <mess/mess.h>

#include <doctest/doctest.h>

/**
 * @brief Test constant and static storage variable.
 */
const int kTheAnswer = 42;

struct FromConstant:
	mess::IsTheConstant<kTheAnswer>
{};
struct FromStaticVariable:
	mess::IsStoredIn<&kTheAnswer>
{};
struct FromLocalVariable:
	mess::IsALocalVariable
{};

/**
 * @brief Test non-member, static, overloaded and template functions.
 */
constexpr int test_function()
{
	return kTheAnswer;
}
struct FromFunction:
	mess::IsTheResultOfCalling<&test_function>,
	mess::WithNoArgument
{};

constexpr static int test_static_function()
{
	return kTheAnswer;
}
struct FromStaticFunction:
	mess::IsTheResultOfCalling<&test_static_function>,
	mess::WithNoArgument
{};

constexpr int test_overload_function()
{
	return kTheAnswer;
}
unsigned int test_overload_function(unsigned int=kTheAnswer)
{
	return kTheAnswer;
}
struct FromOverloadedFunction:
	mess::IsTheResultOfCalling<static_cast<int(*)()>(test_overload_function)>,
	mess::WithNoArgument
{};

template<typename T>
constexpr T test_template_function()
{
	return kTheAnswer;
}
struct FromTemplateFunction:
	mess::IsTheResultOfCalling<&test_template_function<int>>,
	mess::WithNoArgument
{};

/**
 * @brief Test static member functions.
 */
struct test_static_class
{
	constexpr static int test_static_member_function()
	{
		return kTheAnswer;
	}
	constexpr static int test_static_member_overloaded_function()
	{
		return kTheAnswer;
	}
	static unsigned int test_static_member_overloaded_function(unsigned int=kTheAnswer)
	{
		return kTheAnswer;
	}
};
struct FromStaticMemberFunction:
	mess::IsTheResultOfCalling<&test_static_class::test_static_member_function>,
	mess::WithNoArgument
{};
struct FromStaticOverloadedMemberFunction:
	mess::IsTheResultOfCalling<static_cast<int(*)()>(&test_static_class::test_static_member_overloaded_function)>,
	mess::WithNoArgument
{};

/**
 * @brief Test non-const, const and overloaded member functions.
 */
struct test_class
{
	constexpr int test_member_function()
	{
		return kTheAnswer;
	}
	constexpr int test_member_const_function() const
	{
		return kTheAnswer;
	}
	constexpr int test_member_overloaded_function() const
	{
		return kTheAnswer;
	}
	unsigned int test_member_overloaded_function(unsigned int=kTheAnswer) const
	{
		return kTheAnswer;
	}
};
test_class test_instance;
struct TestInstance:
	mess::IsStoredIn<&test_instance>
{};
struct FromMemberFunction:
	mess::IsTheResultOfCalling<&test_class::test_member_function>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};
struct FromMemberConstFunction:
	mess::IsTheResultOfCalling<&test_class::test_member_const_function>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};
struct FromMemberConstOverloadedFunction:
	mess::IsTheResultOfCalling<static_cast<int(test_class::*)()const>(&test_class::test_member_overloaded_function)>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};

auto test_non_capturing_static_lambda = [](){return kTheAnswer;};
struct FromNonCapturingStaticLambda:
	mess::IsTheResultOfCalling<&test_non_capturing_static_lambda>,
	mess::WithNoArgument
{};
constexpr auto test_copy_capture_static_lambda = [theAnswer = kTheAnswer](){return theAnswer;};
struct FromCopyCaptureStaticLambda:
	mess::IsTheResultOfCalling<&test_copy_capture_static_lambda>,
	mess::WithNoArgument
{};
constexpr auto test_ref_capture_static_lambda = [&theAnswer = kTheAnswer](){return theAnswer;};
struct FromRefCaptureStaticLambda:
	mess::IsTheResultOfCalling<&test_ref_capture_static_lambda>,
	mess::WithNoArgument
{};
struct FromLocalLambda:
	mess::IsTheResultOfALocalLambda,
	mess::WithNoArgument
{};

TEST_CASE_TEMPLATE_DEFINE("mess::pull a single value", T, PullTests)
{
	const auto theAnswer = kTheAnswer;
	const auto lambda = [&theAnswer](){return theAnswer;};

	const auto value = mess::pull<T>(
		mess::push<FromLocalVariable>(theAnswer),
		mess::push<FromLocalLambda>(lambda)
		);
	CHECK_EQ(value, kTheAnswer);
}
TEST_CASE_TEMPLATE_DEFINE("constexpr mess::pull a single value", T, ConstexprPullTests)
{
	constexpr auto theAnswer = kTheAnswer;
	constexpr auto lambda = [theAnswer](){return theAnswer;};

	constexpr auto value = mess::pull<T>(
		mess::push<FromLocalVariable>(theAnswer),
		mess::push<FromLocalLambda>(lambda)
		);
	static_assert(value == kTheAnswer, "Pull from constexpr function test failed!");
}
using TestCases = std::tuple<
	FromConstant, FromStaticVariable, FromLocalVariable,
	FromFunction, FromStaticFunction, FromOverloadedFunction, FromTemplateFunction,
	FromStaticMemberFunction, FromStaticOverloadedMemberFunction,
	FromMemberFunction, FromMemberConstFunction, FromMemberConstOverloadedFunction,
	FromNonCapturingStaticLambda, FromCopyCaptureStaticLambda, FromRefCaptureStaticLambda, FromLocalLambda
	>;
TEST_CASE_TEMPLATE_APPLY(PullTests, TestCases);
TEST_CASE_TEMPLATE_APPLY(ConstexprPullTests,  TestCases);
