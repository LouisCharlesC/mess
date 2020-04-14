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

static constexpr int kTheAnswer = 42;

constexpr int test_function()
{
    return kTheAnswer;
}
struct FromFunction:
    mess::Call<test_function>,
    mess::WithNoArgumentss
{};

constexpr static int test_static_function()
{
    return kTheAnswer;
}
struct FromStaticFunction:
    mess::Call<test_static_function>,
    mess::WithNoArgumentss
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
    mess::Call<static_cast<int(*)()>(test_overload_function)>,
    mess::WithNoArgumentss
{};

template<typename T>
constexpr T test_template_function()
{
    return kTheAnswer;
}
struct FromTemplateFunction:
    mess::Call<test_template_function<int>>,
    mess::WithNoArgumentss
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
    mess::Call<&test_static_class::test_static_member_func>,
    mess::WithNoArgumentss
{};
struct FromStaticOverloadedMemberFunction:
    mess::Call<static_cast<int(*)()>(&test_static_class::test_static_member_overloaded_func)>,
    mess::WithNoArgumentss
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
    mess::Call<test_instance>,
    mess::WithNoArgumentss
{};
struct FromMemberFunction:
    mess::Call<&test_class::test_member_func>,
    mess::OnInstance<TestInstance>,
    mess::WithNoArgumentss
{};
struct FromMemberConstFunction:
    mess::Call<&test_class::test_member_const_func>,
    mess::OnInstance<TestInstance>,
    mess::WithNoArgumentss
{};
struct FromMemberOverloadedFunction:
    mess::Call<static_cast<int(test_class::*)()>(&test_class::test_member_overloaded_func)>,
    mess::OnInstance<TestInstance>,
    mess::WithNoArgumentss
{};

constexpr int test_one_dependency_function(int i)
{
    return i;
}
struct FromOneDependency:
    mess::Call<test_one_dependency_function>,
    mess::WithArguments<FromFunction>
{};

constexpr int test_two_dependency_function(int i, int j)
{
    return (i+j)/2;
}
struct FromTwoDependency:
    mess::Call<test_two_dependency_function>,
    mess::WithArguments<FromFunction, FromFunction>
{};
struct FromMultiLevelDependency:
    mess::Call<test_two_dependency_function>,
    mess::WithArguments<FromFunction, FromTwoDependency>
{};

TEST_CASE_TEMPLATE_DEFINE("mess::pull", T, PullTestId)
{
    const auto value = mess::pull<T>();
    CHECK_EQ(value, kTheAnswer);
}
TEST_CASE_TEMPLATE_DEFINE("constexpr mess::pull", T, ConstexprPullTestId)
{
    constexpr auto value = mess::pull<T>();
    static_assert(value == kTheAnswer, "Pull from constexpr function test failed!");
}

using TestCases = std::tuple<
    FromFunction, FromStaticFunction, FromOverloadedFunction, FromTemplateFunction,
    FromStaticMemberFunction, FromStaticOverloadedMemberFunction,
    FromMemberFunction, FromMemberConstFunction, FromMemberOverloadedFunction,
    FromOneDependency, FromTwoDependency, FromMultiLevelDependency>;

TEST_CASE_TEMPLATE_APPLY(PullTestId, TestCases);
TEST_CASE_TEMPLATE_APPLY(ConstexprPullTestId,  TestCases);
