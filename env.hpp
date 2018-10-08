#ifndef ENV_H 
#define ENV_H

#include <boost/optional.hpp>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace ast 
{
	using boost::optional;

	struct Scope
	{
		template<typename T>
		void declare(const std::string&, const optional<T>);

		int getValue(const std::string&, const optional<unsigned int> = boost::none) const;

		void assignValue(std::string, const int&, const optional<unsigned int> = boost::none);

		void insertValue(const std::string&, const optional<int>);
		void insertValue(const std::string&, const optional<std::vector<int>>);

		bool hasVariable(const std::string&) const;


		std::unordered_map<std::string, optional<int>> ints;
		std::unordered_map<std::string, optional< std::vector<int> >> intVecs;

		std::unordered_set<std::string> declaredNames;
	};



	struct Environment
	{
		template<typename T>
		void declare(const std::string&, const optional<T>);

		int getValue(const std::string&, optional<unsigned int> idx = boost::none) const;

		template<typename T>
		void assignValue(const std::string&, const T& value, optional<unsigned int> id = boost::none);

		void createScope();
		void deleteScope();


		std::list<Scope> scopes;
	};


	template<typename T>
	void Environment::declare(const std::string& name, const optional<T> value)
	{
		scopes.back().declare(name, value);
	}

	template<typename T>
	void Environment::assignValue(const std::string& name, const T& value, const optional<unsigned int> idx)
	{
		auto res = std::find_if(scopes.rbegin(), scopes.rend(),
				[&name](const Scope& s){ return s.hasVariable(name);}
				);


		if(res == scopes.rend())
			throw std::runtime_error("Cannot find variable with name "+name);


		res->assignValue(name, value, idx);
	}

	template<typename T>
	void Scope::declare(const std::string& name, const optional<T> value)
	{
		if(!hasVariable(name))
		{
			declaredNames.insert(name);
			insertValue(name, value);
		}
		else
			throw std::runtime_error("Name "+name+" is already in use.");
	}


}

#endif
