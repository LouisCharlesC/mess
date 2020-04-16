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

static const int kTheAnswer = 42;
struct FromConstant:
	mess::IsTheConstant<kTheAnswer>
{};
struct FromPointer:
	mess::IsPulledFrom<&kTheAnswer>
{};

constexpr int test_function()
{
	return kTheAnswer;
}
struct FromFunction:
	mess::IsPulledFrom<test_function>,
	mess::WithNoArgument
{};

constexpr static int test_static_function()
{
	return kTheAnswer;
}
struct FromStaticFunction:
	mess::IsPulledFrom<test_static_function>,
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
	mess::IsPulledFrom<static_cast<int(*)()>(test_overload_function)>,
	mess::WithNoArgument
{};

template<typename T>
constexpr T test_template_function()
{
	return kTheAnswer;
}
struct FromTemplateFunction:
	mess::IsPulledFrom<test_template_function<int>>,
	mess::WithNoArgument
{};

struct test_static_class
{
	constexpr static int test_static_member_func()
	{
		return kTheAnswer;
	}
	constexpr static int test_static_member_overloaded_func()
	{
		return kTheAnswer;
	}
	static unsigned int test_static_member_overloaded_func(unsigned int=kTheAnswer)
	{
		return kTheAnswer;
	}
};
struct FromStaticMemberFunction:
	mess::IsPulledFrom<&test_static_class::test_static_member_func>,
	mess::WithNoArgument
{};
struct FromStaticOverloadedMemberFunction:
	mess::IsPulledFrom<static_cast<int(*)()>(&test_static_class::test_static_member_overloaded_func)>,
	mess::WithNoArgument
{};

struct test_class
{
	constexpr int test_member_func()
	{
		return kTheAnswer;
	}
	constexpr int test_member_const_func() const
	{
		return kTheAnswer;
	}
	constexpr int test_member_overloaded_func()
	{
		return kTheAnswer;
	}
	unsigned int test_member_overloaded_func(unsigned int=kTheAnswer)
	{
		return kTheAnswer;
	}
};
constexpr test_class test_instance()
{
	return test_class();
}
struct TestInstance:
	mess::IsPulledFrom<test_instance>,
	mess::WithNoArgument
{};
struct FromMemberFunction:
	mess::IsPulledFrom<&test_class::test_member_func>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};
struct FromMemberConstFunction:
	mess::IsPulledFrom<&test_class::test_member_const_func>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};
struct FromMemberOverloadedFunction:
	mess::IsPulledFrom<static_cast<int(test_class::*)()>(&test_class::test_member_overloaded_func)>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};

constexpr int test_one_dependency_function(int i)
{
	return i;
}
struct FromOneDependency:
	mess::IsPulledFrom<test_one_dependency_function>,
	mess::WithArgument<FromTemplateFunction>
{};
struct FromConstantDependency:
	mess::IsPulledFrom<test_one_dependency_function>,
	mess::WithArgument<mess::IsTheConstant<kTheAnswer>>
{};

constexpr int test_two_dependency_function(int i, int j)
{
	return (i+j)/2;
}
struct FromTwoDependency:
	mess::IsPulledFrom<test_two_dependency_function>,
	mess::WithArguments<FromStaticOverloadedMemberFunction, FromMemberConstFunction>
{};
struct FromMultiLevelDependency:
	mess::IsPulledFrom<test_two_dependency_function>,
	mess::WithArguments<FromConstant, FromTwoDependency>
{};

TEST_CASE_TEMPLATE_DEFINE("::mess::pull a single value", T, PullTestId)
{
	const auto value = ::mess::pull<T>();
	CHECK_EQ(value, kTheAnswer);
}
TEST_CASE_TEMPLATE_DEFINE("constexpr ::mess::pull a single value", T, ConstexprPullTestId)
{
	constexpr auto value = ::mess::pull<T>();
	static_assert(value == kTheAnswer, "Pull from constexpr function test failed!");
}
TEST_CASE("::mess::pull<...> multiple values")
{
	SUBCASE("pull 2 values")
	{
		const auto [value1, value2] = ::mess::pullAsTuple<FromConstant, FromConstant>();
		CHECK_EQ(value1, kTheAnswer);
		CHECK_EQ(value2, kTheAnswer);
	}
	SUBCASE("pull 3 values")
	{
		const auto [value1, value2, value3] = ::mess::pullAsTuple<FromConstant, FromConstant, FromConstant>();
		CHECK_EQ(value1, kTheAnswer);
		CHECK_EQ(value2, kTheAnswer);
		CHECK_EQ(value3, kTheAnswer);
	}
}

using TestCases = std::tuple<
	FromConstant, FromPointer,
	FromFunction, FromStaticFunction, FromOverloadedFunction, FromTemplateFunction,
	FromStaticMemberFunction, FromStaticOverloadedMemberFunction,
	FromMemberFunction, FromMemberConstFunction, FromMemberOverloadedFunction,
	FromOneDependency, FromConstantDependency, FromTwoDependency, FromMultiLevelDependency>;

TEST_CASE_TEMPLATE_APPLY(PullTestId, TestCases);
TEST_CASE_TEMPLATE_APPLY(ConstexprPullTestId,  TestCases);
