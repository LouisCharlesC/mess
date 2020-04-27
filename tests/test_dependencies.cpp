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

const int kTheAnswer = 42;
struct FromStaticVariable:
	mess::IsStoredIn<&kTheAnswer>
{};

struct test_base_class
{
	virtual int test_virtual_member_function() = 0;

	int m_theAnswer = kTheAnswer;
};
struct test_derived_class: test_base_class
{
	constexpr int test_member_function() const
	{
		return m_theAnswer;
	}
	int test_virtual_member_function() override
	{
		return m_theAnswer++;
	}
};
struct TestInstance:
	mess::IsALocalVariable
{};
struct FromMemberFunction:
	mess::IsTheResultOfCalling<&test_derived_class::test_member_function>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};
struct FromVirtualMemberFunction:
	mess::IsTheResultOfCalling<&test_base_class::test_virtual_member_function>,
	mess::OnInstance<TestInstance>,
	mess::WithNoArgument
{};
struct FromAnotherName:
	mess::Is<FromVirtualMemberFunction>
{};

constexpr int test_function(int i, int j)
{
	return (i+j)-kTheAnswer;
}
struct FromFunction:
	mess::IsTheResultOfCalling<&test_function>,
	mess::WithArguments<FromStaticVariable, FromMemberFunction>
{};
struct FromFunctionThroughVirtual:
	mess::IsTheResultOfCalling<&test_function>,
	mess::WithArguments<FromAnotherName, FromVirtualMemberFunction>
{};

TEST_CASE("mess::pull with dependencies")
{
	test_derived_class test_instance;
	const auto value = mess::pull<FromFunctionThroughVirtual>(mess::push<TestInstance>(static_cast<test_base_class*>(&test_instance)));
	CHECK_EQ(value, kTheAnswer+1);
}
TEST_CASE("constexpr mess::pull with dependencies")
{
	constexpr test_derived_class test_instance;
	constexpr auto value = mess::pull<FromFunction>(mess::push<TestInstance>(test_instance));
	static_assert(value == kTheAnswer, "Pull from constexpr function test failed!");
}