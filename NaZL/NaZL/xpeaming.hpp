// Copyright plasma-effect 2017.
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
#pragma once
#include<variant>
#include<memory>
#include<limits>
#include<vector>
#include<tuple>
#include<functional>
#include<map>
#include<optional>
#include<algorithm>
#include<string>
#include<type_traits>

namespace xpeaming
{
	namespace detail
	{
		template<class>struct type_tag {};
		template<class...>struct type_tuple {};
		template<class Callable, class T, std::size_t... Is>auto invokei(Callable&& callable, T const& val, std::index_sequence<Is...>)
		{
			return callable(std::get<Is>(val)...);
		}
		template<class Callable, class... Args>auto invoke(Callable&& callable, std::tuple<Args...>const& val)
		{
			return invokei(callable, val, std::make_index_sequence<sizeof...(Args)>());
		}
		template<class T>using return_type = std::optional<std::pair<T, std::size_t>>;
		template<class T>return_type<T> make(T&& val, std::size_t i)
		{
			return std::make_pair(std::move(val), i);
		}
		template<class T, class... Ts>std::variant<T, Ts...> variant_add(std::variant<Ts...>&& val)
		{
			return std::visit([](auto&& v) {return std::variant<T, Ts...>(std::move(v)); }, std::move(val));
		}
		template<class T, class U, class... Us>struct exist_check
		{
			static constexpr std::size_t index = exist_check<T, Us...>::index + 1;
		};
		template<class T, class... Us>struct exist_check<T, T, Us...>
		{
			static constexpr std::size_t index = 0;
		};
		template<class T, class U>struct exist_check<T, U>
		{
			static constexpr std::size_t index = 1;
		};
		template<class T>struct exist_check<T, T>
		{
			static constexpr std::size_t index = 0;
		};
	}

	template<char Min, char Max>struct range
	{
		char c;
		operator char()const
		{
			return c;
		}
	};
	template<char Val>using char_t = range<Val, Val>;

	template<char... Cs>struct string
	{
		std::string str;
	};

	template<class T, std::size_t Min = 0, std::size_t Max = std::numeric_limits<std::size_t>::max()>struct repeat
	{
		std::vector<T> value;
		auto begin()const
		{
			return value.begin();
		}
		auto end()const
		{
			return value.end();
		}
	};

	template<char Min, char Max, std::size_t CMin, std::size_t CMax>
	struct repeat<range<Min, Max>, CMin, CMax>
	{
		std::string val;
		auto begin()const
		{
			return val.begin();
		}
		auto end()const
		{
			return val.end();
		}
		std::string const& str()const
		{
			return val;
		}
	};
	template<char Min0, char Max0, std::size_t CMin, std::size_t CMax>
	struct repeat<std::variant<range<Min0, Max0>>, CMin, CMax>
	{
		std::string val;
		auto begin()const
		{
			return val.begin();
		}
		auto end()const
		{
			return val.end();
		}
		std::string const& str()const
		{
			return val;
		}
	};
	template<char Min0, char Max0, char Min1, char Max1, std::size_t CMin, std::size_t CMax>
	struct repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>>, CMin, CMax>
	{
		std::string val;
		auto begin()const
		{
			return val.begin();
		}
		auto end()const
		{
			return val.end();
		}
		std::string const& str()const
		{
			return val;
		}
	};
	template<char Min0, char Max0, char Min1, char Max1, char Min2, char Max2, std::size_t CMin, std::size_t CMax>
	struct repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>>, CMin, CMax>
	{
		std::string val;
		auto begin()const
		{
			return val.begin();
		}
		auto end()const
		{
			return val.end();
		}
		std::string const& str()const
		{
			return val;
		}
	};
	template<char Min0, char Max0, char Min1, char Max1, char Min2, char Max2, char Min3, char Max3, std::size_t CMin, std::size_t CMax>
	struct repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>, range<Min3, Max3>>, CMin, CMax>
	{
		std::string val;
		auto begin()const
		{
			return val.begin();
		}
		auto end()const
		{
			return val.end();
		}
		std::string const& str()const
		{
			return val;
		}
	};

	template<class T>struct predicate
	{
		T val;
	};
	template<class T>struct not_pred
	{

	};

	template<class Return, class... Args>struct registry
	{
		bool space_skip;
		std::function<Return(Args...)> constructor;
		typedef Return return_type;

		template<class T>struct parsing_object
		{
			T& recur;
			std::function<Return(Args...)> const& constructor;
			bool space_skip;
			std::map<std::size_t, std::optional<std::pair<std::shared_ptr<Return>, std::size_t>>> memo;
			detail::return_type<std::shared_ptr<Return>> parse(detail::type_tag<Return>, std::string const* sp, std::size_t i)
			{
				const auto& str = *sp;
				if (memo.count(i))
				{
					return memo[i];
				}
				if (auto ret = recur.parse(detail::type_tag<std::tuple<Args...>>(), str, i, space_skip))
				{
					memo[i] = std::make_pair(std::make_shared<Return>(detail::invoke(constructor, ret->first)), ret->second);
				}
				return memo[i];
			}
			std::nullopt_t parse(...)
			{
				return std::nullopt;
			}
		};
		template<class T>parsing_object<T> make_parsing_object(T& recur)const
		{
			return parsing_object<T>{recur, constructor, space_skip, {}};
		}
	};
	template<class Return, class... Args>struct registry<Return, std::size_t, Args...>
	{
		bool space_skip;
		std::function<Return(std::size_t, Args...)> constructor;
		typedef Return return_type;

		template<class T>struct parsing_object
		{
			T& recur;
			std::function<Return(std::size_t, Args...)> const& constructor;
			bool space_skip;
			std::map<std::size_t, std::optional<std::pair<std::shared_ptr<Return>, std::size_t>>> memo;
			detail::return_type<std::shared_ptr<Return>> parse(detail::type_tag<Return>, std::string const* sp, std::size_t i)
			{
				auto const str = *sp;
				if (memo.count(i))
				{
					return memo[i];
				}
				if (auto ret = recur.parse(detail::type_tag<std::tuple<Args...>>(), str, i, space_skip))
				{
					memo[i] = detail::make(std::make_shared<Return>(detail::invoke(constructor, std::tuple_cat(std::make_tuple(i), ret->first))), ret->second);
				}
				return memo[i];
			}
			std::nullopt_t parse(...)
			{
				return std::nullopt;
			}
		};
		template<class T>parsing_object<T> make_parsing_object(T& recur)const
		{
			return parsing_object<T>{recur, constructor, space_skip, {}};
		}
	};

	template<class Return, class... Args>registry<Return, Args...> regist(std::function<Return(Args...)> cons, bool space_skip = true)
	{
		return registry<Return, Args...>{space_skip, cons};
	}
	template<class Return, class... Args>registry<Return, Args...> regist(Return(*func)(Args...), bool space_skip = true)
	{
		return registry<Return, Args...>{space_skip, func};
	}

	template<class Start, class... Rs>struct parser
	{
		template<class>struct parsing_object;
		std::tuple<Rs...> values;
		std::function<bool(char)> is_space;

		template<std::size_t... Is>struct parsing_object<std::index_sequence<Is...>>
		{
			std::tuple<typename Rs::template parsing_object<parsing_object>...> variable;
			std::function<bool(char)> is_space;

			parsing_object(std::function<bool(char)> space, Rs const&... args):variable(args.make_parsing_object(*this)...), is_space(space)
			{

			}
			template<char Min, char Max>auto parse(
				detail::type_tag<range<Min, Max>>,
				std::string const& str, std::size_t i, bool)
				->detail::return_type<range<Min, Max>>
			{
				if (i != str.size() && Min <= str[i] && str[i] <= Max)
				{
					return std::make_pair(range<Min, Max>{str[i]}, i + 1);
				}
				return std::nullopt;
			}
			template<char Min0, char Max0, std::size_t CMin, std::size_t CMax>auto parse(
				detail::type_tag<repeat<range<Min0, Max0>, CMin, CMax>>,
				std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<repeat<range<Min0, Max0>, CMin, CMax>>
			{
				auto start = i;
				for (std::size_t c = 0; c < CMax; ++c, ++i)
				{
					for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
					if (i == str.size() || (str[i] < Min0 || Max0 < str[i]))
					{
						if (c >= CMin)
						{
							break;
						}
						else
						{
							return std::nullopt;
						}
					}
				}
				return detail::make(repeat<range<Min0, Max0>, CMin, CMax>{str.substr(start, i - start) }, i);
			}
			template<char Min0, char Max0, std::size_t CMin, std::size_t CMax>auto parse(
				detail::type_tag<repeat<std::variant<range<Min0, Max0>>, CMin, CMax>>,
				std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<repeat<std::variant<range<Min0, Max0>>, CMin, CMax>>
			{
				auto start = i;
				for (std::size_t c = 0; c < CMax; ++c, ++i)
				{
					for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
					if (i == str.size() || (str[i] < Min0 || Max0 < str[i]))
					{
						if (c >= CMin)
						{
							break;
						}
						else
						{
							return std::nullopt;
						}
					}
				}
				return detail::make(repeat<std::variant<range<Min0, Max0>>, CMin, CMax>{str.substr(start, i - start) }, i);
			}
			template<char Min0, char Max0, char Min1, char Max1, std::size_t CMin, std::size_t CMax>auto parse(
				detail::type_tag<repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>>, CMin, CMax>>,
				std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>>, CMin, CMax>>
			{
				auto start = i;
				for (std::size_t c = 0; c < CMax; ++c, ++i)
				{
					for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
					if (i == str.size() || (
						(str[i] < Min0 || Max0 < str[i]) &&
						(str[i] < Min1 || Max1 < str[i])))
					{
						if (c >= CMin)
						{
							break;
						}
						else
						{
							return std::nullopt;
						}
					}
				}
				return make(repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>>, CMin, CMax>{str.substr(start, i - start) }, i);
			}
			template<char Min0, char Max0, char Min1, char Max1, char Min2, char Max2, std::size_t CMin, std::size_t CMax>auto parse(
				detail::type_tag<repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>>, CMin, CMax>>,
				std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>>, CMin, CMax>>
			{
				auto start = i;
				for (std::size_t c = 0; c < CMax; ++c, ++i)
				{
					for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
					if (i == str.size() || (
						(str[i] < Min0 || Max0 < str[i]) &&
						(str[i] < Min1 || Max1 < str[i]) &&
						(str[i] < Min2 || Max2 < str[i])))
					{
						if (c >= CMin)
						{
							break;
						}
						else
						{
							return std::nullopt;
						}
					}
				}
				return make(repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>>, CMin, CMax>{str.substr(start, i - start) }, i);
			}
			template<char Min0, char Max0, char Min1, char Max1, char Min2, char Max2, char Min3, char Max3, std::size_t CMin, std::size_t CMax>auto parse(
				detail::type_tag<repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>, range<Min3, Max3>>, CMin, CMax>>,
				std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>, range<Min3, Max3>>, CMin, CMax>>
			{
				auto start = i;
				for (std::size_t c = 0; c < CMax; ++c, ++i)
				{
					for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
					if (i == str.size() || (
						(str[i] < Min0 || Max0 < str[i]) &&
						(str[i] < Min1 || Max1 < str[i]) &&
						(str[i] < Min2 || Max2 < str[i]) &&
						(str[i] < Min3 || Max3 < str[i])))
					{
						if (c >= CMin)
						{
							break;
						}
						else
						{
							return std::nullopt;
						}
					}
				}
				return detail:: make(repeat<std::variant<range<Min0, Max0>, range<Min1, Max1>, range<Min2, Max2>, range<Min3, Max3>>, CMin, CMax>{str.substr(start, i - start) }, i);
			}

			template<char... Cs>auto parse(detail::type_tag<string<Cs...>>, std::string const& str, std::size_t i, bool)
				->detail::return_type<string<Cs...>>
			{
				constexpr char cstr[] = { Cs... };
				if (sizeof...(Cs)+i <= str.size())
				{
					if (std::equal(std::begin(cstr), std::end(cstr), str.begin() + i, str.begin() + sizeof...(Cs)+i))
					{
						return detail::make(string<Cs...>{std::string(cstr)}, i + sizeof...(Cs));
					}
				}
				return std::nullopt;
			}

			template<class T, std::size_t Min, std::size_t Max>auto parse(detail::type_tag<repeat<T, Min, Max>>, std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<repeat<T, Min, Max>>
			{
				std::vector<T> ret;
				for (std::size_t c = 0; c < Max; ++c)
				{
					for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
					if (auto v = parse(detail::type_tag<T>(), str, i, space_skip))
					{
						ret.push_back(std::move(v->first));
						i = v->second;
					}
					else if (c >= Min)
					{
						break;
					}
					else
					{
						return std::nullopt;
					}
				}
				return detail::make(repeat<T, Min, Max>{std::move(ret)}, i);
			}

			template<class T>auto parse(detail::type_tag<std::variant<T>>, std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<std::variant<T>>
			{
				for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
				if (auto ret = parse(detail::type_tag<T>(), str, i, space_skip))
				{
					return detail::make(std::variant<T>(std::move(ret->first)), ret->second);
				}
				return std::nullopt;
			}
			template<class T, class... Ts>auto parse(detail::type_tag<std::variant<T, Ts...>>, std::string const& str, std::size_t i, bool space_skip)
				->std::enable_if_t<sizeof...(Ts) != 0, detail::return_type<std::variant<T, Ts...>>>
			{
				for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
				if (auto ret = parse(detail::type_tag<T>(), str, i, space_skip))
				{
					return detail::make(std::variant<T, Ts...>(std::move(ret->first)), ret->second);
				}
				else if (auto next = parse(detail::type_tag<std::variant<Ts...>>(), str, i, space_skip))
				{
					return detail::make(detail::variant_add<T>(std::move(next->first)), next->second);
				}
				return std::nullopt;
			}

			template<class T>auto parse(detail::type_tag<std::tuple<T>>, std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<std::tuple<T>>
			{
				for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
				if (auto ret = parse(detail::type_tag<T>(), str, i, space_skip))
				{
					return detail::make(std::make_tuple(ret->first), ret->second);
				}
				return std::nullopt;
			}
			template<class T, class... Ts>auto parse(detail::type_tag<std::tuple<T, Ts...>>, std::string const& str, std::size_t i, bool space_skip)
				->std::enable_if_t<sizeof...(Ts) != 0, detail::return_type<std::tuple<T, Ts...>>>
			{
				for (; space_skip&& i < str.size() && is_space(str[i]); ++i);
				if (auto ret = parse(detail::type_tag<T>(), str, i, space_skip))
				{
					if (auto next = parse(detail::type_tag<std::tuple<Ts...>>(), str, ret->second, space_skip))
					{
						return detail::make(std::tuple_cat(std::make_tuple(ret->first), next->first), next->second);
					}
				}
				return std::nullopt;
			}

			template<class T>auto parse(detail::type_tag<std::optional<T>>, std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<std::optional<T>>
			{
				if (auto ret = parse(detail::type_tag<T>(), str, i, space_skip))
				{
					return detail::make(std::make_optional(ret->first), ret->second);
				}
				return detail::make(std::optional<T>(), i);
			}

			template<class T>auto parse(detail::type_tag<predicate<T>>, std::string const& str, std::size_t i, bool space_skip)
				->detail::return_type<predicate<T>>
			{
				if (auto ret = parse(detail::type_tag<T>(), str, i, space_skip))
				{
					return detail::make(predicate<T>{std::move(ret->first)}, i);
				}
				return std::nullopt;
			}
			template<class T>auto parse(detail::type_tag<not_pred<T>>, std::string const& str, std::size_t i, bool space_skip)
			{
				if (!parse(detail::type_tag<T>(), str, i, space_skip))
				{
					return detail::make(not_pred<T>{}, i);
				}
				return std::nullopt;
			}

			template<class T>auto parse(detail::type_tag<std::shared_ptr<T>>, std::string const& str, std::size_t i, bool)
				->std::enable_if_t<detail::exist_check<T, typename Rs::return_type...>::index != sizeof...(Rs), detail::return_type<std::shared_ptr<T>>>
			{
				return std::get<detail::exist_check<T, typename Rs::return_type...>::index>(variable).parse(detail::type_tag<T>(), &str, i);
			}

			template<class T>auto parse(detail::type_tag<std::shared_ptr<T>>, std::string const&, std::size_t, bool)
				->std::enable_if_t<detail::exist_check<T, typename Rs::return_type...>::index == sizeof...(Rs), detail::return_type<std::shared_ptr<T>>>
			{
				static_assert(!std::is_same_v<T, T>, "have not be registed constructor for [T]");
				throw 0;
			}
		};

		std::shared_ptr<Start> operator()(std::string const& str)const
		{
			parsing_object<std::make_index_sequence<sizeof...(Rs)>> p(is_space, std::get<Rs>(values)...);
			if (auto ret = p.parse(detail::type_tag<std::shared_ptr<Start>>(), str, 0, false))
			{
				return ret->first;
			}
			return nullptr;
		}
	};

	namespace detail
	{
		template<class T>struct check_regist :std::false_type
		{

		};
		template<class... Ts>struct check_regist<registry<Ts...>> :std::true_type
		{

		};
		template<class R, class... Rs>struct check_argument
		{
			static constexpr bool value = check_regist<R>::value && check_argument<Rs...>::value;
		};
		template<class R>struct check_argument<R>
		{
			static constexpr bool value = check_regist<R>::value;
		};
	}

	template<class Start, class... Rs>auto make_parser(Rs&&... args)
		->std::enable_if_t<detail::check_argument<Rs...>::value, parser<Start, Rs...>>
	{
		return parser<Start, Rs...>{std::make_tuple(args...), [](char c) {return c == ' ' || c == '\t'; }};
	}

	template<class Start, class... Rs>auto make_parser(Rs&&...)
		->std::enable_if_t<!detail::check_argument<Rs...>::value, std::function<std::shared_ptr<Start>(std::string const&)>>
	{
		static_assert(detail::check_argument<Rs...>::value, "arguments must be converted to [xpeaming::registry] by [xpeaming::regist]");
		throw 0;
	}

}