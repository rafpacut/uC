#include "ast.h"

namespace ast {

	struct varDecl
	{
            std::string name;
            int value;
	};

	struct Expr
	{
		int val;
	};


        struct statement : x3::variant<
		varDecl,
		Expr	
	>
	{
		using base_type::base_type;
		using base_type::operator=;
        };

    struct program {
        std::list<statement> stmts;
    };
} 

BOOST_FUSION_ADAPT_STRUCT(ast::Expr, 
		(int,val)
		)

BOOST_FUSION_ADAPT_STRUCT(ast::varDecl, 
		(std::string, name)
         	,(int, value)
		)

BOOST_FUSION_ADAPT_STRUCT(ast::program, stmts)
