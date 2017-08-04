#include"NaZL.hpp"
#include<iostream>
#include<fstream>

int main()
{
	std::string filename;
	std::getline(std::cin, filename);
	std::vector<std::exception_ptr> exceptions;
	auto run = NaZL::runner(std::fstream(filename), exceptions);
	if (exceptions.size() != 0)
	{
		for (auto const& exc : exceptions)
		{
			try
			{
				std::rethrow_exception(exc);
			}
			catch(std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
	else
	{
		std::int64_t val;
		auto func = run.get_function<std::int64_t>("test");
		try
		{
			while (true)
			{
				std::cin >> val;
				std::cout << func(val) << std::endl;
			}
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}