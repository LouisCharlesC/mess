/**
 * @file flow.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <mess/ops.h>

namespace mess
{
	template<auto Value>
	struct Names
	{
		static constexpr Operation op = Operation::Get;
		static constexpr auto ValueToRead = Value;
	};
	template<auto Value>
	using Unnamed = Names<Value>;

	template<typename Name>
	struct Invokes
	{
		static constexpr Operation op = Operation::Invoke;
		using NameToInvoke = Name;
	};

	template<typename... Names>
	struct WithArguments {};
	using WithNoArgument = WithArguments<>;
	template<typename Name>
	using WithArgument = WithArguments<Name>;

	template<typename Name>
	struct Dereferences
	{
		static constexpr Operation op = Operation::Dereference;
		using NameToDereference = Name;
	};

	template<typename Name>
	struct Replaces
	{
		using NameToReplace = Name;
	};

	template<typename... Names>
	struct When
	{

	};
} // namespace mess