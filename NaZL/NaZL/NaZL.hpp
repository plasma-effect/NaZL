#pragma once
#include"syntax_tree.hpp"
#include"utility.hpp"
#include<any>
#include<stack>

namespace NaZL
{
	namespace inside_command
	{
		namespace detail
		{
			template<std::size_t>struct value_type_emerge
			{
				typedef utility::value_type type;
			};
			template<class>struct value_func_emerge;
			template<std::size_t... Is>struct value_func_emerge<std::index_sequence<Is...>>
			{
				typedef std::function<utility::value_type(typename value_type_emerge<Is>::type...)> type;
			};
		}
		template<std::size_t Size>struct function_call
		{
			typedef typename detail::value_func_emerge<std::make_index_sequence<Size>>::type type;
			type func;
		};
		struct literal_push
		{
			utility::value_type value;
		};
		struct value_push
		{
			std::size_t index;
		};
		struct value_return
		{

		};
		struct pop_assign
		{
			std::size_t index;
		};
		typedef std::variant< 
			function_call<0>, 
			function_call<1>,
			function_call<2>,
			function_call<3>,
			function_call<4>,
			function_call<5>,
			function_call<6>,
			function_call<7>,
			function_call<8>,
			function_call<9>, 
			literal_push, value_push, value_return, pop_assign> command;
	}

	class runner
	{
		std::map<std::string, std::pair<std::size_t, std::vector<inside_command::command>>> functions[10];
		std::map<std::string, inside_command::function_call<0>::type> embedded_function0;
		std::map<std::string, inside_command::function_call<1>::type> embedded_function1;
		std::map<std::string, inside_command::function_call<2>::type> embedded_function2;
		std::map<std::string, inside_command::function_call<3>::type> embedded_function3;
		std::map<std::string, inside_command::function_call<4>::type> embedded_function4;
		std::map<std::string, inside_command::function_call<5>::type> embedded_function5;
		std::map<std::string, inside_command::function_call<6>::type> embedded_function6;
		std::map<std::string, inside_command::function_call<7>::type> embedded_function7;
		std::map<std::string, inside_command::function_call<8>::type> embedded_function8;
		std::map<std::string, inside_command::function_call<9>::type> embedded_function9;
		void run(inside_command::literal_push const& lite, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			stack.push(lite.value);
		}
		void run(inside_command::value_push const& val, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			stack.push(local[val.index]);
		}
		void run(inside_command::value_return const& val, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			ret = stack.top();
			stack.pop();
		}
		void run(inside_command::pop_assign const& val, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			if (local.size() <= val.index)
			{
				local.resize(val.index + 1);
			}
			local[val.index] = stack.top();
			stack.pop();
		}
		void run(inside_command::function_call<0> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			stack.push(func.func());
		}
		void run(inside_command::function_call<1> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			stack.push(func.func(arg0));
		}
		void run(inside_command::function_call<2> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			stack.push(func.func(arg0, arg1));
		}
		void run(inside_command::function_call<3> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			auto arg2 = stack.top(); stack.pop();
stack.push(func.func(arg0, arg1, arg2));
		}
		void run(inside_command::function_call<4> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			auto arg2 = stack.top(); stack.pop();
			auto arg3 = stack.top(); stack.pop();
			stack.push(func.func(arg0, arg1, arg2, arg3));
		}
		void run(inside_command::function_call<5> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			auto arg2 = stack.top(); stack.pop();
			auto arg3 = stack.top(); stack.pop();
			auto arg4 = stack.top(); stack.pop();
			stack.push(func.func(arg0, arg1, arg2, arg3, arg4));
		}
		void run(inside_command::function_call<6> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			auto arg2 = stack.top(); stack.pop();
			auto arg3 = stack.top(); stack.pop();
			auto arg4 = stack.top(); stack.pop();
			auto arg5 = stack.top(); stack.pop();
			stack.push(func.func(arg0, arg1, arg2, arg3, arg4, arg5));
		}
		void run(inside_command::function_call<7> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			auto arg2 = stack.top(); stack.pop();
			auto arg3 = stack.top(); stack.pop();
			auto arg4 = stack.top(); stack.pop();
			auto arg5 = stack.top(); stack.pop();
			auto arg6 = stack.top(); stack.pop();
			stack.push(func.func(arg0, arg1, arg2, arg3, arg4, arg5, arg6));
		}
		void run(inside_command::function_call<8> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			auto arg2 = stack.top(); stack.pop();
			auto arg3 = stack.top(); stack.pop();
			auto arg4 = stack.top(); stack.pop();
			auto arg5 = stack.top(); stack.pop();
			auto arg6 = stack.top(); stack.pop();
			auto arg7 = stack.top(); stack.pop();
			stack.push(func.func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
		}
		void run(inside_command::function_call<9> const& func, utility::sub_vector& local, std::stack<utility::value_type>& stack, std::optional<utility::value_type>& ret, std::size_t&)const
		{
			auto arg0 = stack.top(); stack.pop();
			auto arg1 = stack.top(); stack.pop();
			auto arg2 = stack.top(); stack.pop();
			auto arg3 = stack.top(); stack.pop();
			auto arg4 = stack.top(); stack.pop();
			auto arg5 = stack.top(); stack.pop();
			auto arg6 = stack.top(); stack.pop();
			auto arg7 = stack.top(); stack.pop();
			auto arg8 = stack.top(); stack.pop();
			stack.push(func.func(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
		}

		utility::value_type run(std::vector<inside_command::command> const& coms, utility::sub_vector local)const
		{
			std::stack<utility::value_type> stack;
			std::optional<utility::value_type> ret;
			for (std::size_t line = 0; line < coms.size(); ++line)
			{
				std::visit([&](auto const& com) {this->run(com, local, stack, ret, line); }, coms[line]);
				if (ret)
				{
					return *ret;
				}
			}
			return std::int64_t();
		}
		
		struct function_define
		{
			runner& run;
			std::vector<inside_command::command>& commands;
			std::map<std::string, std::size_t>& local;
			std::size_t& line_count;
			void make_inside_command(syntax_tree::additional_ptr const& expr)
			{
				make_inside_command(expr->first);
				for (auto const& es : expr->nexts)
				{
					make_inside_command(es);
					commands.push_back(inside_command::function_call<2>{ run.embedded_function2.at("operator+__int__int") });
				}
			}
			void make_inside_command(syntax_tree::multi_ptr const& expr)
			{
				make_inside_command(expr->first);
				for (auto const& es : expr->nexts)
				{
					make_inside_command(es);
					commands.push_back(inside_command::function_call<2>{ run.embedded_function2.at("operator*__int__int") });
				}
			}
			void make_inside_command(syntax_tree::nliteral_ptr const& expr)
			{
				commands.push_back(inside_command::literal_push{ utility::make_value(expr->val) });
			}
			void make_inside_command(syntax_tree::value_ptr const& expr)
			{
				if (local.count(expr->name))
				{
					commands.push_back(inside_command::value_push{ local.at(expr->name) });
				}
				else
				{
					throw std::invalid_argument(std::to_string(line_count + 1) + ", " + std::to_string(expr->column + 1) + ": ïœêî[" + expr->name + "]Ç™íËã`Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒ");
				}
			}
			void make_inside_command(syntax_tree::parent_ptr const& expr)
			{
				make_inside_command(expr->expr);
			}
			void make_inside_command(syntax_tree::expression_ptr const& expr)
			{
				std::visit([&](auto const& ptr) {this->make_inside_command(ptr); }, expr->expr);
			}

			void make_inside_command(syntax_tree::value_define_ptr const& define)
			{
				if (local.count(define->name))
				{
					throw std::invalid_argument(utility::place_string(line_count, 1) + "Ç∑Ç≈Ç…ïœêî[" + define->name + "]ÇÕíËã`Ç≥ÇÍÇƒÇ¢Ç‹Ç∑");
				}
				make_inside_command(define->init);
				commands.push_back(inside_command::pop_assign{ local.size() });
				local.emplace(define->name, local.size());
			}
			void make_inside_command(syntax_tree::value_assign_ptr const& define)
			{
				if (!local.count(define->name))
				{
					throw std::invalid_argument(utility::place_string(line_count, 1) + "ïœêî[" + define->name + "]ÇÕíËã`Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒ");
				}
				make_inside_command(define->expr);
				commands.push_back(inside_command::pop_assign{ local[define->name] });
			}
			void make_inside_command(syntax_tree::expr_return_ptr const& expr)
			{
				make_inside_command(expr->expr);
				commands.push_back(inside_command::value_return{});
			}
		};

		void make_inside_command(syntax_tree::function_define const& define, std::vector<std::exception_ptr>& exceptions)
		{
			std::vector<inside_command::command> implement;
			std::map<std::string, std::size_t> local;
			for (auto const& a : define.declare->argument)
			{
				local.emplace(a.name, local.size());
			}
			std::size_t line_count = 0;
			function_define definition{ *this,implement,local,line_count };
			for (auto const& p : define.implement)
			{
				line_count = p.first;
				try
				{
					std::visit([&](auto const& ptr) {definition.make_inside_command(ptr); }, p.second->value);
				}
				catch (...)
				{
					exceptions.emplace_back(std::current_exception());
				}
			}
			switch (define.declare->argument.size())
			{
			case 0:
				functions[0].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 1:
				functions[1].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 2:
				functions[2].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 3:
				functions[3].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 4:
				functions[4].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 5:
				functions[5].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 6:
				functions[6].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 7:
				functions[7].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 8:
				functions[8].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			case 9:
				functions[9].emplace(define.declare->name, std::make_pair(local.size(), std::move(implement)));
				break;
			}
		}


	public:
		template<class Return>struct running_object
		{
			runner const& obj;
			std::string name;
			template<class... Ts>Return operator()(Ts const&... args)const
			{
				if (obj.functions[sizeof...(args)].count(name))
				{
					auto const& pair = obj.functions[sizeof...(args)].at(name);
					std::vector<utility::value_type> local = { utility::make_value(args)... };
					local.resize(pair.first);
					auto sub = utility::sub_vector(local, 0);
					return utility::value_type_cast<Return>(obj.run(pair.second, sub));
				}
				else
				{
					throw std::invalid_argument("äYìñÇ∑ÇÈä÷êîÇ™ë∂ç›ÇµÇ»Ç¢Ç©à¯êîÇÃêîÇ™çáÇ¢Ç‹ÇπÇÒ");
				}
			}
		};
		template<class Return>running_object<Return> get_function(std::string name)
		{
			return running_object<Return>{*this, std::move(name)};
		}

		runner(std::istream&& stream, std::vector<std::exception_ptr>& exceptions):
			functions{},
			embedded_function0{},
			embedded_function1{},
			embedded_function2{},
			embedded_function3{},
			embedded_function4{},
			embedded_function5{},
			embedded_function6{},
			embedded_function7{},
			embedded_function8{},
			embedded_function9{}
		{
			auto syntax_tree = syntax_tree::make_syntax_tree(std::move(stream), exceptions);
			embedded_function2.emplace("operator+__int__int", utility::make_inside_function(std::function<std::int64_t(std::int64_t, std::int64_t)>(std::plus<>())));
			embedded_function2.emplace("operator*__int__int", utility::make_inside_function(std::function<std::int64_t(std::int64_t, std::int64_t)>(std::multiplies<>())));
			for (auto const& tree : syntax_tree)
			{
				std::visit([&](auto const& t) {this->make_inside_command(t, exceptions); }, tree);
			}
		}
	};
}