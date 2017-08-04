#pragma once
#include"syntax_tree.hpp"
#include<iostream>

namespace NaZL_test
{
	namespace syntax_tree
	{
		void print(NaZL::syntax_tree::atomic_ptr const& atomic)
		{
			std::cout << atomic->name;
		}

		void print(NaZL::syntax_tree::expression_ptr const& expr);
		void print(NaZL::syntax_tree::nliteral_ptr const& nlit)
		{
			std::cout << nlit->val;
		}
		void print(NaZL::syntax_tree::value_ptr const& val)
		{
			std::cout << val->name;
		}
		void print(NaZL::syntax_tree::parent_ptr const& par)
		{
			print(par->expr);
		}
		void print(NaZL::syntax_tree::multi_ptr const& mul)
		{
			if (mul->nexts.size() == 0)
			{
				print(mul->first);
			}
			else
			{
				std::cout << "(* ";
				print(mul->first);
				for (auto const& n : mul->nexts)
				{
					std::cout << " ";
					print(n);
				}
				std::cout << ")";
			}
		}
		void print(NaZL::syntax_tree::additional_ptr const& add)
		{
			if (add->nexts.size() == 0)
			{
				print(add->first);
			}
			else
			{
				std::cout << "(+ ";
				print(add->first);
				for (auto const& n : add->nexts)
				{
					std::cout << " ";
					print(n);
				}
				std::cout << ")";
			}
		}
		void print(NaZL::syntax_tree::expression_ptr const& expr)
		{
			std::visit([](auto const& p) {print(p); }, expr->expr);
		}

		void print(NaZL::syntax_tree::value_define_ptr const& def)
		{
			std::cout << "    value define: " << def->name << std::endl;
			std::cout << "     initializer: ";
			print(def->init);
			std::cout << std::endl;
		}
		void print(NaZL::syntax_tree::value_assign_ptr const& ass)
		{
			std::cout << "    value assign: " << ass->name << std::endl;
			std::cout << "      expression: ";
			print(ass->expr);
			std::cout << std::endl;
		}
		void print(NaZL::syntax_tree::expr_return_ptr const& ret)
		{
			std::cout << "    return: ";
			print(ret->expr);
			std::cout << std::endl;
		}

		void print(NaZL::syntax_tree::function_define const& define)
		{
			std::cout << "line: " << define.line << std::endl;
			std::cout << " function define" << std::endl;
			std::cout << "  function name: " << define.declare->name << std::endl;
			std::cout << "  arguments:" << std::endl;
			for (auto const& v : define.declare->argument)
			{
				std::cout << "   " << v.name << ": ";
				std::visit([](auto const& a) {print(a); }, v.type->value);
				std::cout << std::endl;
			}
			std::cout << "  implement: " << std::endl;
			for (auto const& p : define.implement)
			{
				std::cout << "   line: " << p.first << std::endl;
				std::visit([](auto const& p) {print(p); }, p.second->value);
			}
		}

		void print(std::vector<NaZL::syntax_tree::syntax_tree>const& vec)
		{
			for (auto const& v : vec)
			{
				std::visit([](auto const& p) {print(p); }, v);
			}
		}
	}
}