#pragma once
#include<stdexcept>
#include<variant>
#include<any>
#include<vector>
#include<functional>
#include<string>

namespace NaZL
{
	namespace utility
	{
		[[noreturn]]void throw_no_implement()
		{
			throw std::logic_error("ñ¢é¿ëïÇ≈Ç∑(êªçÏé“Ç…òAóçÇµÇƒÇ≠ÇæÇ≥Ç¢)");
		}
		typedef std::variant<std::int64_t, std::any> value_type;
		namespace detail
		{
			template<class T>struct value_type_cast_i
			{
				static T const& cast(value_type const& val)
				{
					return std::any_cast<T>(std::get<std::any>(val));
				}
			};
			template<>struct value_type_cast_i<std::int64_t>
			{
				static std::int64_t const& cast(value_type const& val)
				{
					return std::get<std::int64_t>(val);
				}
			};
		}
		template<class T>T const& value_type_cast(value_type const& val)
		{
			return detail::value_type_cast_i<T>::cast(val);
		}
		value_type make_value(std::int64_t val)
		{
			return value_type(val);
		}
		template<class T>value_type make_value(T const& val)
		{
			return value_type(std::any(val));
		}
		namespace detail
		{
			template<class>struct value_type_trans
			{
				typedef value_type type;
			};

			template<class Return, class... Args>struct make_inside_function_i
			{
				static value_type impl(std::size_t);
				typedef std::function<value_type(typename value_type_trans<Args>::type...)> type;
				static type make(std::function<Return(Args...)>func)
				{
					return [=](typename value_type_trans<Args>::type... args)
					{
						return make_value(func(value_type_cast<Args>(args)...));
					};
				}
			};
		}
		template<class Return, class... Args>auto make_inside_function(std::function<Return(Args...)> func)
		{
			return detail::make_inside_function_i<Return, Args...>::make(func);
		}

		class sub_vector
		{
			std::vector<utility::value_type>& vec;
			std::size_t shift;
		public:
			sub_vector(std::vector<utility::value_type>& v, std::size_t s = 0):vec(v), shift(s)
			{

			}
			utility::value_type& operator[](std::size_t index)
			{
				return vec[index + shift];
			}
			utility::value_type const& operator[](std::size_t index)const
			{
				return vec[index + shift];
			}
			void resize(std::size_t size)
			{
				vec.resize(size + shift);
			}
			sub_vector get_sub_vector(std::size_t start)
			{
				return sub_vector(vec, start + shift);
			}
			std::size_t size()
			{
				return vec.size() - shift;
			}
		};

		std::string place_string(std::size_t line, std::size_t column)
		{
			return std::to_string(line + 1) + ", " + std::to_string(column + 1) + ": ";
		}
	}
}