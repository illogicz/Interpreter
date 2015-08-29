#pragma once

#include "IEvalable.h"
#include "TokenStream.h"

class Statement;
class Program;

class Parser {
	public:
		Parser();
		Program* parse(TokenStream* ts);

	private:
		TokenStream* ts;

		Statement* statement();
		Statement* compound_statement();
		Statement* for_statement();
		Statement* while_statement();
		Statement* if_statement();
		Statement* return_statement();

		IEvalable::Uptr expression();
		IEvalable::Uptr relational_expression();
		IEvalable::Uptr additive_expression();
		IEvalable::Uptr multiplicative_expression();
		IEvalable::Uptr unary_expression();
		IEvalable::Uptr primary();
		IEvalable::Uptr function();
		IEvalable::Uptr function_call(IEvalable::Uptr exp);

}; 