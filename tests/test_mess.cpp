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

int test_function()
{
    return 32;
}

static int test_static_function()
{
    return 32;
}

constexpr int test_constexpr_function()
{
    return 32;
}

int test_overload_function()
{
    return 32;
}

unsigned int test_overload_function(unsigned int=32)
{
    return 32;
}

template<typename T>
T test_template_function()
{
    return 32;
}

struct test_class
{
    static int test_static_member_func()
    {
        return 32;
    }
    static int test_static_overloaded_member_func()
    {
        return 32;
    }
    static unsigned int test_static_overloaded_member_func(unsigned int=32)
    {
        return 32;
    }
};

struct FromFunction {};
struct FromStaticFunction {};
struct FromConstexprFunction {};
struct FromOverloadedFunction {};
struct FromTemplateFunction {};
struct FromStaticMemberFunction {};
struct FromStaticOverloadedMemberFunction {};

namespace mess
{
template<> struct
ToProduce<FromFunction>:
    Call<test_function>,
    WithNoArgs
{};
template<> struct
ToProduce<FromStaticFunction>:
    Call<test_static_function>,
    WithNoArgs
{};
template<> struct
ToProduce<FromConstexprFunction>:
    Call<test_constexpr_function>,
    WithNoArgs
{};
template<> struct
ToProduce<FromOverloadedFunction>:
    Call<static_cast<int(*)()>(test_overload_function)>,
    WithNoArgs
{};
template<> struct
ToProduce<FromTemplateFunction>:
    Call<test_template_function<int>>,
    WithNoArgs
{};
template<> struct
ToProduce<FromStaticMemberFunction>:
    Call<&test_class::test_static_member_func>,
    WithNoArgs
{};
template<> struct
ToProduce<FromStaticOverloadedMemberFunction>:
    Call<static_cast<int(*)()>(&test_class::test_static_overloaded_member_func)>,
    WithNoArgs
{};
}

TEST_CASE("Pull from function")
{
    const auto value = mess::Frame<>::pull<FromFunction>();
    CHECK_EQ(value, int(32));
}

TEST_CASE("Pull from static function")
{
    const auto value = mess::Frame<>::pull<FromStaticFunction>();
    CHECK_EQ(value, int(32));
}

TEST_CASE("Pull from constexpr function")
{
    constexpr auto value = mess::Frame<>::pull<FromConstexprFunction>();
    static_assert(value == 32, "Pull from constexpr function test failed!");
}

TEST_CASE("Pull from overloaded function")
{
    const auto value = mess::Frame<>::pull<FromOverloadedFunction>();
    CHECK_EQ(value, int(32));
}

TEST_CASE("Pull from template function")
{
    const auto value = mess::Frame<>::pull<FromTemplateFunction>();
    CHECK_EQ(value, int(32));
}

TEST_CASE("Pull from static member function")
{
    const auto value = mess::Frame<>::pull<FromStaticMemberFunction>();
    CHECK_EQ(value, int(32));
}

TEST_CASE("Pull from static overloaded member function")
{
    const auto value = mess::Frame<>::pull<FromStaticOverloadedMemberFunction>();
    CHECK_EQ(value, int(32));
}
