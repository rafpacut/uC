#ifndef ENV_H 
#define ENV_H

#include <boost/optional.hpp>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "ast.h"

namespace ast 
{
	using boost::optional;

	struct Scope
	{
		template<typename T>
		void declare(const std::string&, const T);

		basicType getValue(const std::string&, const optional<size_t> = boost::none) const;

		void assignValue(const std::string, const int, const size_t);
		void assignValue(const std::string, const std::vector<int>, const size_t);
		void assignValue(const std::string, const std::vector<int>);
		void assignValue(const std::string, const int);

		void insertValue(const std::string&, const optional<int>);
		void insertValue(const std::string&, const optional<std::vector<int>>);

		void arrayExtend(std::string const&, int const);
		int arraySize(std::string const&) const;

		bool hasVariable(const std::string&) const;

		std::unordered_map<std::string, optional<int>> ints;
		std::unordered_map<std::string, optional< std::vector<int> >> intVecs;

		std::unordered_set<std::string> declaredNames;
	};



	struct Environment
	{
		using Function = FunctionDecl;

		Environment();
		Environment(const Environment& e);
		Environment& operator=(const Environment&) = default;
		Environment& operator=(Environment&&);

		template<typename T>
		void declare(const std::string&, const T);
		void declare(const Function&);

		basicType getValue(const std::string&, optional<size_t> idx = boost::none) const;

		template<typename T>
		void assignValue(const std::string&, const T&, optional<size_t> id = boost::none);

		template<typename T>
		void arrayExtend(std::string const&, T const&);

		int arraySize(std::string const&) const;

		void createScope();
		void deleteScope();

		std::list<Scope> scopes;
		std::set<Function> functions;

		bool returnStatementEvald = false;
	};

	template<typename T>
	void Environment::arrayExtend(std::string const& name, T const& value)
	{
		auto res = std::find_if(scopes.rbegin(), scopes.rend(),
				[&name](const Scope& s){ return s.hasVariable(name);}
				);

		res->arrayExtend(name, value);
	}

	template<typename T>
	void Environment::declare(const std::string& name, const T value)
	{
		scopes.back().declare(name, value);
	}

	template<typename T>
	void Environment::assignValue(const std::string& name, const T& value, const optional<size_t> idx)
	{
		auto res = std::find_if(scopes.rbegin(), scopes.rend(),
				[&name](const Scope& s){ return s.hasVariable(name);}
				);

		if(res == scopes.rend())
			throw std::runtime_error("Cannot find variable with name "+name);

		if(idx)
			res->assignValue(name, value, *idx);
		else
			res->assignValue(name, value);
	}

	template<typename T>
	void Scope::declare(const std::string& name, const T value)
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
