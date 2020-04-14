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

int test_function()
{
    return kTheAnswer;
}

static int test_static_function()
{
    return kTheAnswer;
}

constexpr int test_constexpr_function()
{
    return kTheAnswer;
}

int test_overload_function()
{
    return kTheAnswer;
}

unsigned int test_overload_function(unsigned int=kTheAnswer)
{
    return kTheAnswer;
}

template<typename T>
T test_template_function()
{
    return kTheAnswer;
}

struct test_class
{
    static int test_static_member_func()
    {
        return kTheAnswer;
    }
    static int test_static_overloaded_member_func()
    {
        return kTheAnswer;
    }
    static unsigned int test_static_overloaded_member_func(unsigned int=kTheAnswer)
    {
        return kTheAnswer;
    }
    int test_member_func()
    {
        return kTheAnswer;
    }
    int test_member_const_func() const
    {
        return kTheAnswer;
    }
};

test_class test_instance()
{
    return test_class();
}

constexpr int test_dependent_constexpr_function(int i)
{
    return i;
}

struct FromFunction:
    mess::Call<test_function>,
    mess::WithNoArgumentss
{};
struct FromStaticFunction:
    mess::Call<test_static_function>,
    mess::WithNoArgumentss
{};
struct FromConstexprFunction:
    mess::Call<test_constexpr_function>,
    mess::WithNoArgumentss
{};
struct FromOverloadedFunction:
    mess::Call<static_cast<int(*)()>(test_overload_function)>,
    mess::WithNoArgumentss
{};
struct FromTemplateFunction:
    mess::Call<test_template_function<int>>,
    mess::WithNoArgumentss
{};
struct FromStaticMemberFunction:
    mess::Call<&test_class::test_static_member_func>,
    mess::WithNoArgumentss
{};
struct FromStaticOverloadedMemberFunction:
    mess::Call<static_cast<int(*)()>(&test_class::test_static_overloaded_member_func)>,
    mess::WithNoArgumentss
{};
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
struct FromDependent:
    mess::Call<test_dependent_constexpr_function>,
    mess::WithArguments<FromFunction>
{};
struct FromConstexprDependent:
    mess::Call<test_dependent_constexpr_function>,
    mess::WithArguments<FromConstexprFunction>
{};

TEST_CASE("Pull from function")
{
    const auto value = mess::Frame::pull<FromFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from static function")
{
    const auto value = mess::Frame::pull<FromStaticFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from constexpr function")
{
    constexpr auto value = mess::Frame::pull<FromConstexprFunction>();
    static_assert(value == kTheAnswer, "Pull from constexpr function test failed!");
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from overloaded function")
{
    const auto value = mess::Frame::pull<FromOverloadedFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from template function")
{
    const auto value = mess::Frame::pull<FromTemplateFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from static member function")
{
    const auto value = mess::Frame::pull<FromStaticMemberFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from static overloaded member function")
{
    const auto value = mess::Frame::pull<FromStaticOverloadedMemberFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from member function")
{
    const auto value = mess::Frame::pull<FromMemberFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from member const function")
{
    const auto value = mess::Frame::pull<FromMemberConstFunction>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from dependent function")
{
    const auto value = mess::Frame::pull<FromDependent>();
    CHECK_EQ(value, kTheAnswer);
}

TEST_CASE("Pull from constexpr dependent function")
{
    constexpr auto value = mess::Frame::pull<FromConstexprFunction>();
    static_assert(value == kTheAnswer, "Pull from constexpr dependent function test failed!");
    CHECK_EQ(value, kTheAnswer);
}