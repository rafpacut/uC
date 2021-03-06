#include "astPrinter.h"


namespace ast
{
        void Printer::operator()(unsigned int n) const { std::cout << n; }
	void Printer::operator()(int n) const {std::cout<<n;}

	void Printer::operator()(std::string s) const
	{
		std::cout<<"(variable '"<<s<<"')";
	}

        void Printer::operator()(Operation const& x) const
        {
		std::cout<<"(";
            boost::apply_visitor(*this, x.operand_);
            switch (x.operator_)
            {
                case '+': std::cout << " add"; break;
                case '-': std::cout << " subt"; break;
                case '*': std::cout << " mult"; break;
                case '/': std::cout << " div"; break;
            }
		std::cout<<")";
        }

	void Printer::operator()(ArraySize const& x) const
	{
		std::cout<<"array '"<<x.name<<"' size\n";
	}

	void Printer::operator()(PushBack const& x) const
	{
		std::cout<<"Extending Array '"<<x.name<<"' with ";
		(*this)(x.value);
		std::cout<<'\n';
	}

        void Printer::operator()(Signed_ const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': std::cout << " neg"; break;
                case '+': std::cout << " pos"; break;
            }
        }

	void Printer::operator()(Comparison const& x) const
	{
		(*this)(x.lhs);
		std::cout<<x.op;
		(*this)(x.rhs);
	}

	void Printer::operator()(Print const& x) const
	{
		std::cout<<"(Print ";
		(*this)(x.val);
		std::cout<<")\n";
	}

	void Printer::operator()(VarDecl const& x) const
	{
		std::cout<<"(Variable Declaration: name = '"<<x.name<<"' value = (";
		if(x.value)
			(*this)(*(x.value));
		std::cout<<")\n";
	}

	void Printer::operator()(ArrDecl const& x) const
	{
		std::cout<<"(Array Declaration: name = '"<<x.name<<"'";
		if(x.initValue)
		{
			std::cout<<" value =(";
			(*this)(*(x.initValue));
		}
		std::cout<<")\n";
	}

	void Printer::operator()(ArrValue const& x) const
	{
		std::cout<<"(Array name = '"<<x.name<<"' at(";
		(*this)(x.idx);
		std::cout<<"))\n";
	}

	void Printer::operator()(Conditional const& x) const
	{
		std::cout<<"(IF ";
		(*this)(x.condition);
		(*this)(x.tBody);

		if(x.fBody)
		{
			std::cout<<"\n ELSE \n";
			(*this)(*(x.fBody));
		}
		std::cout<<")\n";
	}

	void Printer::operator()(Assignment const& x) const
	{
		std::cout<<"Assigning value(";
		(*this)(x.value);
		std::cout<<") to "<<x.name<<'\n';
	}

	void Printer::operator()(AssignmentArr const& x) const
	{
		std::cout<<"Assigning value(";
		(*this)(x.value);
		std::cout<<") to ";
		(*this)(x.id);
		std::cout<<'\n';
	}

	void Printer::operator()(Expr const& x) const
	{
		boost::apply_visitor(*this, x.first);
		for(const Operation& o : x.rest)
		{
			std::cout<<' ';
			(*this)(o);
		}
	}

	void Printer::operator()(WhileLoop const& x) const
	{
		std::cout<<"While loop (";
		(*this)(x.condition);
		std::cout<<')';
		(*this)(x.body);
	}

	void Printer::operator()(Statement const& x) const
	{
		boost::apply_visitor(*this, x);
	}

	void Printer::operator()(Return const& x) const
	{
		std::cout<<"return ";
		(*this)(x.value);
		std::cout<<std::endl;
	}

	void Printer::operator()(FunctionDecl const& x) const
	{
		std::cout<<"Function declaration\n";
		std::cout<<"name:"<<x.name;
		std::cout<<"\nargs:\n";
		if(x.args)
		{
			auto& args = *(x.args);
			for(const auto& a: args)
				boost::apply_visitor(*this, a);
		}

		(*this)(x.body);
	}

	void Printer::operator()(FunctionCall const& x) const
	{
		std::cout<<"Function call ";
		std::cout<<x.name<<'(';
		if(x.params)
		{
			auto& params = *(x.params);
			for(size_t i = 0; i < params.size(); ++i)
			{
				(*this)(params[i]);
				std::cout<<", ";
			}
		}
		std::cout<<")\n";

	}

	void Printer::operator()(std::list<Statement> const& x) const
	{
		std::cout<<"\n{\n";
		for(Statement const& stmt : x)
		{
			(*this)(stmt);
		}
		std::cout<<"\n}\n";
	}

}









