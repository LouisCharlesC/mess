/**
 * @file mess.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <tuple>

namespace mess
{
	enum class From
	{
		Constant,
		PointerToStaticVariable,
		NonMemberFunction,
		MemberFunction,
		StaticLambda,
		LocalLambda,
		LocalVariable,
		Name,
	};

	template<typename ArgsTuple, typename... Pushed>
	class Push
	{
		template<typename P, std::size_t... Indexes>
		static constexpr std::size_t getIndex(std::index_sequence<Indexes...>)
		{
			using PushedTuple = std::tuple<Pushed...>;
			return ((static_cast<std::size_t>(std::is_same<std::tuple_element_t<Indexes, PushedTuple>, P>::value)*Indexes) + ... + 0);
		}

	public:
		template<typename... Args>
		constexpr Push(Args&&... args):
			m_args(std::forward<Args>(args)...)
		{}

		template<typename P>
		static constexpr bool contains()
		{
			constexpr std::size_t foundNTimes = (static_cast<std::size_t>(std::is_same<P, Pushed>::value) + ... + 0);
			static_assert(foundNTimes <= 1, "Pushed... must be pack of unique types");
			return foundNTimes == 1;
		}

		template<typename P>
		constexpr decltype(auto) get()
		{
			static_assert(Push::contains<P>(), "Check that P is contained in Pushed... with function Push::constains<P>() before clling Push::get<P>()");
			return std::get<Push::getIndex<P>(std::index_sequence_for<Pushed...>())>(m_args);
		}

	private:
		ArgsTuple m_args;
	};
	template<typename Pushed, typename Arg>
	constexpr auto push(Arg&& arg)
	{
		return Push<std::tuple<Arg&&>, Pushed>(std::forward<Arg>(arg));
	}
	template<typename Pushed, typename Arg>
	constexpr auto push(Arg* arg)
	{
		return Push<std::tuple<Arg*>, Pushed>(arg);
	}
	template<typename... Arg, typename... Pushed>
	constexpr auto cat(Push<std::tuple<Arg>, Pushed>&&... pushes)
	{
		return Push<std::tuple<Arg...>, Pushed...>(pushes.template get<Pushed>()...);
	}

	template<typename O, typename... Arg, typename... Pushed>
	constexpr decltype(auto) pull(Push<Arg, Pushed>&&...);

	template<typename... Inputs>
	struct WithArguments
	{
		template<typename O, typename ArgsTuple, typename... Pushed>
		static constexpr decltype(auto) pull(Push<ArgsTuple, Pushed...>&& pushed)
		{
			if constexpr (Push<ArgsTuple, Pushed...>::template contains<O>())
			{
				if constexpr (O::IsWhat == ::mess::From::LocalLambda)
				{
					return (pushed.template get<O>())(Inputs::WithArguments::template pull<Inputs>(std::move(pushed))...);
				}
				else
				{
					return pushed.template get<O>();
				}
			}
			else
			{
				if constexpr (O::IsWhat == ::mess::From::Constant)
				{
					return O::IsHere;
				}
				else if constexpr (O::IsWhat == ::mess::From::PointerToStaticVariable)
				{
					return *O::IsHere;
				}
				else if constexpr (O::IsWhat == ::mess::From::NonMemberFunction)
				{
					return O::IsHere(Inputs::WithArguments::template pull<Inputs>(std::move(pushed))...);
				}
				else if constexpr (O::IsWhat == ::mess::From::MemberFunction)
				{
					if constexpr (std::is_pointer_v<std::remove_reference_t<decltype(O::OnInstance::Instance::WithArguments::template pull<typename O::OnInstance::Instance>(std::move(pushed)))>>)
					{
						return (O::OnInstance::Instance::WithArguments::template pull<typename O::OnInstance::Instance>(std::move(pushed))->*O::IsHere)(Inputs::WithArguments::template pull<Inputs>(std::move(pushed))...);
					}
					else
					{
						return (O::OnInstance::Instance::WithArguments::template pull<typename O::OnInstance::Instance>(std::move(pushed)).*O::IsHere)(Inputs::WithArguments::template pull<Inputs>(std::move(pushed))...);
					}
				}
				else if constexpr (O::IsWhat == ::mess::From::StaticLambda)
				{
					return (*O::IsHere)(Inputs::WithArguments::template pull<Inputs>(std::move(pushed))...);
				}
				else if constexpr (O::IsWhat == ::mess::From::Name)
				{
					return O::IsHere::WithArguments::template pull<typename O::IsHere>(std::move(pushed));
				}
			}
		}
	};
	using WithNoArgument = WithArguments<>;
	template<typename Arg>
	using WithArgument = WithArguments<Arg>;

	template<typename Name>
	struct Is: WithNoArgument
	{
		using IsHere = Name;
		static constexpr ::mess::From IsWhat = ::mess::From::Name;
	};

	template<decltype(auto) Pointer>
	struct IsStoredIn: WithNoArgument
	{
		static constexpr decltype(auto) IsHere = Pointer;
		static constexpr ::mess::From IsWhat = ::mess::From::PointerToStaticVariable;
	};

	struct IsALocalVariable: WithNoArgument
	{
		static constexpr ::mess::From IsWhat = ::mess::From::LocalVariable;
	};

	struct IsTheResultOfALocalLambda
	{
		static constexpr ::mess::From IsWhat = ::mess::From::LocalLambda;
	};

	// Lambdas
	template<decltype(auto) Function>
	struct IsTheResultOfCalling
	{
		static constexpr decltype(auto) IsHere = Function;
		static constexpr ::mess::From IsWhat = ::mess::From::StaticLambda;
	};

	// Non-member function pointers
	template <typename R, typename... Args, R(*Function)(Args...)>
	struct IsTheResultOfCalling<Function>
	{
		static constexpr decltype(auto) IsHere = Function;
		static constexpr ::mess::From IsWhat = ::mess::From::NonMemberFunction;
	};

	// Member function pointers
	template <typename C, typename R, typename... Args, R(C::*Function)(Args...)>
	struct IsTheResultOfCalling<Function>
	{
		static constexpr decltype(auto) IsHere = Function;
		static constexpr ::mess::From IsWhat = ::mess::From::MemberFunction;
	};
	template <typename C, typename R, typename... Args, R(C::*Function)(Args...) const>
	struct IsTheResultOfCalling<Function>
	{
		static constexpr decltype(auto) IsHere = Function;
		static constexpr ::mess::From IsWhat = ::mess::From::MemberFunction;
	};
	// and their instance
	template<typename I>
	struct OnInstance
	{
		using Instance = I;
	};

	// Constants
	template<decltype(auto) Constant>
	struct IsTheConstant: WithNoArgument
	{
		static const decltype(auto) IsHere = Constant;
		static constexpr ::mess::From IsWhat = ::mess::From::Constant;
	};

	template<typename O, typename... Arg, typename... Pushed>
	constexpr decltype(auto) pull(Push<Arg, Pushed>&&... pushes)
	{
		return O::WithArguments::template pull<O>(cat(std::move(pushes)...));
	}
} // namespace mess