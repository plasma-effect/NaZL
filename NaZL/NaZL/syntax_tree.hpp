#pragma once
#include<string>
#include<memory>
#include"xpeaming.hpp"

namespace NaZL
{
	namespace syntax_tree
	{
		using namespace xpeaming;
		typedef std::variant<range<'a', 'z'>, range<'A', 'Z'>, char_t<'_'>> alpha_;
		typedef std::variant<range<'a', 'z'>, range<'A', 'Z'>, range<'0', '9'>, char_t<'_'>> token_;
		struct token
		{
			std::string name;
		};
		auto make_token(alpha_ a, repeat<token_> b)
		{
			return token{ std::visit([](auto const& v) {return v.c; },a) + b.str() };
		}
		typedef std::shared_ptr<token> token_ptr;

		struct value_type;
		typedef std::shared_ptr<value_type> type_ptr;
		struct atomic_type
		{
			std::string name;
		};
		typedef std::shared_ptr<atomic_type> atomic_ptr;
		auto make_atomic_type(token_ptr val)
		{
			return atomic_type{ std::move(val->name) };
		}
		struct value_type
		{
			typedef std::variant<atomic_ptr> type;
			type value;
		};
		auto make_value_type(value_type::type value)
		{
			return value_type{ std::move(value) };
		}


		struct expression;
		typedef std::shared_ptr<expression> expression_ptr;
		struct nliteral
		{
			std::size_t column;
			std::int64_t val;
		};
		struct value
		{
			std::size_t column;
			std::string name;
		};
		struct parenthese
		{
			expression_ptr expr;
		};
		template<char... Op>struct bioperator
		{
			std::size_t column;
			expression_ptr first;
			std::vector<expression_ptr> nexts;
		};
		typedef std::shared_ptr<nliteral> nliteral_ptr;
		typedef std::shared_ptr<value> value_ptr;
		typedef std::shared_ptr<parenthese> parent_ptr;
		typedef std::shared_ptr<bioperator<'+'>> additional_ptr;
		typedef std::shared_ptr<bioperator<'*'>> multi_ptr;
		struct expression
		{
			typedef std::variant<additional_ptr, multi_ptr, parent_ptr, nliteral_ptr, value_ptr> type;
			type expr;
		};
		auto transport_expression(expression::type expr)
		{
			return std::make_shared<expression>(expression{ std::move(expr) });
		}
		auto make_nliteral(std::size_t column, repeat<range<'0', '9'>, 1> val)
		{
			return nliteral{ column, std::stoll(val.str()) };
		}
		auto make_value(std::size_t column, token_ptr val)
		{
			return value{ column,  val->name };
		}
		auto make_parent(char_t<'('>, expression_ptr expr, char_t<')'>)
		{
			return parenthese{ std::move(expr) };
		}
		auto make_addit(std::size_t column, multi_ptr first, repeat<std::tuple<char_t<'+'>, multi_ptr>> seconds)
		{
			std::vector<expression_ptr> vec;
			for (auto const& t : seconds)
			{
				vec.push_back(transport_expression(std::get<1>(t)));
			}
			return bioperator<'+'>{column, transport_expression(first), std::move(vec)};
		}
		auto make_multi(std::size_t column, std::variant<parent_ptr, nliteral_ptr, value_ptr> first, repeat<std::tuple<char_t<'*'>, std::variant<parent_ptr, nliteral_ptr, value_ptr>>> seconds)
		{
			std::vector<expression_ptr> vec;
			for (auto const& t : seconds)
			{
				vec.push_back(std::visit([](auto const& ptr) {return transport_expression(ptr); }, std::get<1>(t)));
			}
			return bioperator<'*'>{column, std::visit([](auto const& ptr) {return transport_expression(ptr); }, first), std::move(vec)};
		}
		auto make_expression(additional_ptr add)
		{
			return expression{ add };
		}

		struct command;
		typedef std::shared_ptr<command> command_ptr;
		struct value_define
		{
			std::string name;
			expression_ptr init;
		};
		struct value_assign
		{
			std::string name;
			expression_ptr expr;
		};
		struct expression_return
		{
			expression_ptr expr;
		};
		typedef std::shared_ptr<value_define> value_define_ptr;
		typedef std::shared_ptr<value_assign> value_assign_ptr;
		typedef std::shared_ptr<expression_return> expr_return_ptr;
		auto make_value_define(string<'v', 'a', 'r', ' '>, token_ptr val, char_t<'='>, expression_ptr init)
		{
			return value_define{ std::move(val->name),std::move(init) };
		}
		auto make_value_assign(token_ptr val, char_t<'='>, expression_ptr init)
		{
			return value_assign{ std::move(val->name),std::move(init) };
		}
		auto make_expression_return(string<'r', 'e', 't', 'u', 'r', 'n', ' '>, expression_ptr expr)
		{
			return expression_return{ std::move(expr) };
		}
		struct command
		{
			typedef std::variant<value_define_ptr, value_assign_ptr, expr_return_ptr> type;
			type value;
		};
		auto make_command(command::type value)
		{
			return command{ std::move(value) };
		}
		
		struct statement;
		typedef std::shared_ptr<statement> statement_ptr;
		struct function_declare
		{
			std::string name;
			struct argument_data
			{
				std::string name;
				type_ptr type;
			};
			std::vector<argument_data> argument;
		};
		typedef std::shared_ptr<function_declare> function_declare_ptr;
		auto make_function_declare(string<'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' '>, 
			token_ptr name, char_t<'('>, 
			std::optional<std::tuple<token_ptr, char_t<':'>, type_ptr,
			repeat<std::tuple<char_t<','>, token_ptr, char_t<':'>, type_ptr>>>> args, char_t<')'>)
		{
			std::vector<function_declare::argument_data> vec;
			if (args)
			{
				vec.push_back(function_declare::argument_data{ std::get<0>(*args)->name,std::get<2>(*args) });
				for (auto const& g : std::get<3>(*args))
				{
					vec.push_back(function_declare::argument_data{ std::get<1>(g)->name,std::get<3>(g) });
				}
			}
			return function_declare{ std::move(name->name),std::move(vec) };
		}
		struct statement
		{
			typedef std::variant<function_declare_ptr> type;
			type value;
		};
		auto make_statement(statement::type value)
		{
			return statement{ std::move(value) };
		}

		struct function_define
		{
			std::size_t line;
			function_declare_ptr declare;
			std::vector<std::pair<std::size_t, command_ptr>> implement;
		};
		typedef std::variant<function_define> syntax_tree;

		auto parse_statement(function_declare_ptr declare, std::istream& stream, std::size_t& line_count, std::vector<std::exception_ptr>& exceptions)
		{
			auto command_parser = make_parser<command>(
				regist(make_token, false),
				regist(make_parent),
				regist(make_addit),
				regist(make_multi),
				regist(make_nliteral, false),
				regist(make_value),
				regist(make_expression),
				regist(make_value_define),
				regist(make_value_assign),
				regist(make_expression_return),
				regist(make_command));
			std::vector<std::pair<std::size_t, command_ptr>> implement;
			std::string line;
			auto start = line_count;
			for (++line_count; std::getline(stream, line); ++line_count)
			{
				if (line == "")
				{
					continue;
				}
				if (line == "end")
				{
					return syntax_tree{ function_define{start, std::move(declare),std::move(implement)} };
				}
				if (auto com = command_parser(line))
				{
					implement.emplace_back(line_count, std::move(com));
				}
				else
				{
					try
					{
						throw std::invalid_argument(std::to_string(line_count + 1) + ", 0: 不明のコマンドです");
					}
					catch (...)
					{
						exceptions.push_back(std::current_exception());
					}
				}
			}
			try
			{
				throw std::invalid_argument(std::to_string(start + 1) + ", 0: 関数定義の終わりのendが見つかりませんでした");
			}
			catch (...)
			{
				exceptions.push_back(std::current_exception());
			}
			return syntax_tree{ function_define{ start, std::move(declare),std::move(implement) } };
		}

		auto make_syntax_tree(std::istream&& stream,std::vector<std::exception_ptr>& exceptions)
		{
			auto statement_parser = make_parser<statement>(
				regist(make_statement), 
				regist(make_function_declare), 
				regist(make_value_type), 
				regist(make_token, false),
				regist(make_atomic_type));
			
			std::string line;
			std::vector<syntax_tree> vec;
			for (std::size_t line_count = 0; std::getline(stream, line); ++line_count)
			{
				if (line == "")
				{
					continue;
				}
				if (auto state = statement_parser(line))
				{
					vec.push_back(std::visit([&](auto rec) {return parse_statement(rec, stream, line_count, exceptions); }, state->value));
				}
				else
				{
					try
					{
						throw std::invalid_argument(std::to_string(line_count + 1) + ", 0: 不明のステートメントです");
					}
					catch (...)
					{
						exceptions.push_back(std::current_exception());
					}
				}
			}
			return vec;
		}
	}
}