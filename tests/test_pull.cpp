/**
 * @file test_pull.cpp
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

struct Constant:
	mess::Names<kTheAnswer>
{};
struct Pointer:
	mess::Names<&kTheAnswer>
{};
struct Dereferenced:
	mess::Dereferences<Pointer>
{};

/**
 * @brief Test non-member, static, overloaded and template functions.
 */
constexpr int test_function()
{
	return kTheAnswer;
}
struct Function:
	mess::Names<&test_function>
{};
struct Invoked:
	mess::Invokes<Function>
{};

// constexpr static int test_static_function()
// {
// 	return kTheAnswer;
// }
// struct StaticFunction:
// 	mess::Is<&test_static_function>
// {};

// constexpr int test_overload_function()
// {
// 	return kTheAnswer;
// }
// unsigned int test_overload_function(unsigned int=kTheAnswer)
// {
// 	return kTheAnswer;
// }
// struct OverloadedFunction:
// 	mess::Is<static_cast<int(*)()>(test_overload_function)>
// {};

// template<typename T>
// constexpr T test_template_function()
// {
// 	return kTheAnswer;
// }
// struct TemplateFunction:
// 	mess::Is<&test_template_function<int>>
// {};

// /**
//  * @brief Test static member functions.
//  */
// struct test_static_class
// {
// 	constexpr static int test_static_member_function()
// 	{
// 		return kTheAnswer;
// 	}
// 	constexpr static int test_static_member_overloaded_function()
// 	{
// 		return kTheAnswer;
// 	}
// 	static unsigned int test_static_member_overloaded_function(unsigned int=kTheAnswer)
// 	{
// 		return kTheAnswer;
// 	}
// };
// struct StaticMemberFunction:
// 	mess::Is<&test_static_class::test_static_member_function>
// {};
// struct StaticOverloadedMemberFunction:
// 	mess::Is<static_cast<int(*)()>(&test_static_class::test_static_member_overloaded_function)>
// {};

// /**
//  * @brief Test non-const, const and overloaded member functions.
//  */
// struct test_class
// {
// 	constexpr int test_member_function()
// 	{
// 		return kTheAnswer;
// 	}
// 	constexpr int test_member_const_function() const
// 	{
// 		return kTheAnswer;
// 	}
// 	constexpr int test_member_overloaded_function() const
// 	{
// 		return kTheAnswer;
// 	}
// 	unsigned int test_member_overloaded_function(unsigned int=kTheAnswer) const
// 	{
// 		return kTheAnswer;
// 	}
// };
// test_class test_instance;
// struct TestInstance:
// 	mess::IsStoredAt<&test_instance>
// {};
// struct MemberFunction:
// 	mess::Is<&test_class::test_member_function>
// {};
// struct MemberConstFunction:
// 	mess::Is<&test_class::test_member_const_function>
// {};
// struct MemberConstOverloadedFunction:
// 	mess::Is<static_cast<int(test_class::*)()const>(&test_class::test_member_overloaded_function)>
// {};

// auto test_non_capturing_static_lambda = [](){return kTheAnswer;};
// struct NonCapturingStaticLambda:
// 	mess::Is<&test_non_capturing_static_lambda>
// {};
// constexpr auto test_copy_capture_static_lambda = [theAnswer = kTheAnswer](){return theAnswer;};
// struct CopyCaptureStaticLambda:
// 	mess::Is<&test_copy_capture_static_lambda>
// {};
// constexpr auto test_ref_capture_static_lambda = [&theAnswer = kTheAnswer](){return theAnswer;};
// struct RefCaptureStaticLambda:
// 	mess::Is<&test_ref_capture_static_lambda>
// {};

TEST_CASE("Pull a constant")
{
	constexpr auto value = mess::pull<Constant>();
	static_assert(value == kTheAnswer, "Failed!");
	CHECK_EQ(value, kTheAnswer);
}
TEST_CASE("Pull a pointer")
{
	constexpr auto value = *mess::pull<Pointer>();
	static_assert(value == kTheAnswer, "Failed!");
	CHECK_EQ(value, kTheAnswer);
}
TEST_CASE("Pull-dereference a pointer")
{
	constexpr auto value = mess::pull<Dereferenced>();
	static_assert(value == kTheAnswer, "Failed!");
	CHECK_EQ(value, kTheAnswer);
}
TEST_CASE("Pull an invocable")
{
	constexpr auto value = mess::pull<Function>()();
	static_assert(value == kTheAnswer, "Failed!");
	CHECK_EQ(value, kTheAnswer);
}
TEST_CASE("Pull-dereference a pointer")
{
	constexpr auto value = mess::pull<Invoked>();
	static_assert(value == kTheAnswer, "Failed!");
	CHECK_EQ(value, kTheAnswer);
}

// TEST_CASE_TEMPLATE_DEFINE("Pull an invocable", T, IncovablesTest)
// {
// 	constexpr auto value = (*mess::pull<T>())();
// 	static_assert(value == kTheAnswer, "Pull from constexpr function test failed!");
// 	CHECK_EQ(value, kTheAnswer);
// }
// using IncovablesTestCases = std::tuple<
// 	Function, StaticFunction, OverloadedFunction, TemplateFunction,
// 	StaticMemberFunction, StaticOverloadedMemberFunction,
// 	// MemberFunction, MemberConstFunction, MemberConstOverloadedFunction,
// 	NonCapturingStaticLambda, CopyCaptureStaticLambda, RefCaptureStaticLambda
// 	>;
// TEST_CASE_TEMPLATE_APPLY(IncovablesTest, IncovablesTestCases);
