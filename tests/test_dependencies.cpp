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

struct test_base_class
{
	constexpr int test_member_function() const
	{
		return m_theAnswer-1;
	}
	virtual int test_virtual_member_function() const
	{
		return m_theAnswer+2;
	}

	int m_theAnswer = kTheAnswer;
};
struct test_derived_class: test_base_class
{
	constexpr int test_member_function() const
	{
		return m_theAnswer+1;
	}
	int test_virtual_member_function() const override
	{
		return m_theAnswer-2;
	}
};
test_derived_class test_derived_instance;
const test_derived_class& test_derived_const_instance=test_derived_instance;
test_derived_class* test_derived_pointer=&test_derived_instance;
const test_derived_class* test_derived_const_pointer=&test_derived_instance;
test_base_class test_base_instance;
const test_base_class& test_base_const_instance=test_base_instance;
test_base_class* test_base_pointer=&test_base_instance;
const test_base_class* test_base_const_pointer=&test_base_instance;
constexpr test_derived_class test_constexpr_derived_instance;
constexpr test_base_class test_constexpr_base_instance;

struct TestDerivedInstance:
	mess::IsStoredIn<&test_derived_instance>
{};
struct TestDerivedConstInstance:
	mess::IsStoredIn<&test_derived_const_instance>
{};
struct TestDerivedPointer:
	mess::IsStoredIn<&test_derived_pointer>
{};
struct TestDerivedConstPointer:
	mess::IsStoredIn<&test_derived_const_pointer>
{};
struct TestBaseInstance:
	mess::IsStoredIn<&test_base_instance>
{};
struct TestBaseConstInstance:
	mess::IsStoredIn<&test_base_const_instance>
{};
struct TestBasePointer:
	mess::IsStoredIn<&test_base_pointer>
{};
struct TestBaseConstPointer:
	mess::IsStoredIn<&test_base_const_pointer>
{};
struct TestConstexprDerivedInstance:
	mess::IsStoredIn<&test_constexpr_derived_instance>
{};
struct TestConstexprBaseInstance:
	mess::IsStoredIn<&test_constexpr_base_instance>
{};

template<typename Instance>
struct FromBaseMemberFunction:
	mess::IsTheResultOfCalling<&test_base_class::test_member_function>,
	mess::OnInstance<Instance>,
	mess::WithNoArgument
{};
template<typename Instance>
struct FromDerivedMemberFunction:
	mess::IsTheResultOfCalling<&test_derived_class::test_member_function>,
	mess::OnInstance<Instance>,
	mess::WithNoArgument
{};
template<typename Instance>
struct FromVirtualMemberFunction:
	mess::IsTheResultOfCalling<&test_base_class::test_virtual_member_function>,
	mess::OnInstance<Instance>,
	mess::WithNoArgument
{};

template<typename Other>
struct FromOther:
	mess::Is<Other>
{};

constexpr int test_function(int i, int j)
{
	return (i+j)-kTheAnswer;
}
template<typename FirstArgument, typename SecondArgument>
struct FromFunction:
	mess::IsTheResultOfCalling<&test_function>,
	mess::WithArguments<FirstArgument, SecondArgument>
{};

template<std::size_t N>
struct FromOtherRecursive:
	FromOtherRecursive<N-1>
{};
template<>
struct FromOtherRecursive<0>:
	mess::IsTheConstant<kTheAnswer>
{};

TEST_CASE_TEMPLATE_DEFINE("mess::pull with dependencies", T, MemberPullTests)
{
	auto value = mess::pull<T>();
	CHECK_EQ(value, kTheAnswer);
}
using TestCases = std::tuple<
	FromFunction<FromDerivedMemberFunction<TestDerivedInstance>, FromBaseMemberFunction<TestBaseInstance>>,
	FromFunction<FromDerivedMemberFunction<TestDerivedConstInstance>, FromBaseMemberFunction<TestBaseConstInstance>>,
	FromFunction<FromDerivedMemberFunction<TestDerivedPointer>, FromBaseMemberFunction<TestBasePointer>>,
	FromFunction<FromDerivedMemberFunction<TestDerivedConstPointer>, FromBaseMemberFunction<TestBaseConstPointer>>,
	FromFunction<FromVirtualMemberFunction<TestDerivedInstance>, FromVirtualMemberFunction<TestBaseInstance>>,
	FromFunction<FromVirtualMemberFunction<TestDerivedConstInstance>, FromVirtualMemberFunction<TestBaseConstInstance>>,
	FromFunction<FromVirtualMemberFunction<TestDerivedPointer>, FromVirtualMemberFunction<TestBasePointer>>,
	FromFunction<FromVirtualMemberFunction<TestDerivedConstPointer>, FromVirtualMemberFunction<TestBaseConstPointer>>,
	FromFunction<mess::Is<mess::IsTheConstant<kTheAnswer>>, mess::Is<mess::IsTheConstant<kTheAnswer>>>,
	FromOtherRecursive<800>
	>;
TEST_CASE_TEMPLATE_APPLY(MemberPullTests, TestCases);

TEST_CASE("constexpr mess::pull with dependencies")
{
	using T = FromFunction<FromDerivedMemberFunction<TestConstexprDerivedInstance>, FromBaseMemberFunction<TestConstexprBaseInstance>>;
	constexpr auto value = mess::pull<T>();
	static_assert(value == kTheAnswer, "Pull from constexpr function test failed!");
}