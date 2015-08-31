#pragma once

#include "IEvalable.h"
#include "TokenStream.h"
#include "Jump.h"

class Statement;
class CompoundStatement;
class Program;

class Parser {
	public:
		Parser();
		Program* parse(TokenStream* ts);

	private:
		TokenStream* ts;

		Statement* statement();
		Statement* block();
		Statement* statement_l();

		Statement* jump_statement(Jump::Type type);
		Statement* try_statement();
		Statement* for_statement();
		Statement* while_statement();
		Statement* if_statement();
		
		IEvalable::Uptr condition();
		IEvalable::Uptr logical_or_expression();
		IEvalable::Uptr logical_and_expression();
		IEvalable::Uptr expression();
		IEvalable::Uptr expression_or_empty();
		IEvalable::Uptr for_expression(Value def);
		IEvalable::Uptr equality_expression();
		IEvalable::Uptr relational_expression();
		IEvalable::Uptr shift_expression();
		IEvalable::Uptr additive_expression();
		IEvalable::Uptr multiplicative_expression();
		IEvalable::Uptr unary_expression();
		IEvalable::Uptr primary();
		IEvalable::Uptr value(Value v);
		IEvalable::Uptr function();
		IEvalable::Uptr function_call(IEvalable::Uptr exp);

}; 