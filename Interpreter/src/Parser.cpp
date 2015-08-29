#include "stdafx.h"
#include "Utils.h"
#include "Parser.h"
#include "Expression.h"
#include "TokenStream.h"
#include "Value.h"
#include "Function.h"
#include "Program.h"

Parser::Parser() {};

Program* Parser::parse(TokenStream* token_stream) {
	ts = token_stream;
	auto* cs = new Program();
	try {
		Token t = ts->get();

		while (true) {
			switch (t.type) {
				case Token::EOFILE: return cs;
				case Token::EOL: break;
				default: {
					ts->putback(t);
					cs->add(statement());
				}
			}
			t = ts->get();
		}
	}
	 catch (exception e){
		 cout << "Parse Error: " << e.what() << endl;
		 return cs;
	 }
}

Statement* Parser::compound_statement()
{
	auto* cs = new CompoundStatement();
	Token t = ts->get();
	while (true) {
		switch (t.type) {
			case Token::CLOSE_BRACE: return cs;
			case Token::EOFILE: my_error("no close brace found");
			case Token::EOL: break;
			default: {
				ts->putback(t);
				cs->add(statement());
			}
		}
		t = ts->get();
	}
}

Statement* Parser::statement()
{
	Token t = ts->get();

	switch (t.type) {
		case Token::EOL: return new EmptyStatement;
		case Token::FOR: return for_statement();
		case Token::WHILE: return while_statement();
		case Token::IF: return if_statement();
		case Token::RETURN: return return_statement();
		case Token::OPEN_BRACE: return compound_statement();
		default: {
			ts->putback(t);
			return new ExpressionStatement(expression()); // expression_statement
		}
	}

}

Statement* Parser::for_statement()
{
	return new CompoundStatement();
}

Statement* Parser::while_statement()
{
	return new CompoundStatement();
}

Statement* Parser::return_statement()
{
	return new ReturnStatement(expression());
}

Statement* Parser::if_statement()
{
	Token t = ts->get();
	if (t.type != Token::OPEN_BRACKET) {
		my_error("expected open bracket after if keyword");
	}

	t = ts->get();
	if (t.type == Token::CLOSE_BRACKET) {
		my_error("expected expression in if statement");
	}
	ts->putback(t);
	auto c = expression();

	t = ts->get();
	if (t.type != Token::CLOSE_BRACKET) {
		my_error("expected closing bracket after if condition");
	}

	Statement* s = statement();

	t = ts->get();
	if (t.type != Token::ELSE) {
		ts->putback(t);
		return new ConditionalStatement(std::move(c), s);
	}

	return new ConditionalStatement(std::move(c), s, statement());

}


IEvalable::Uptr Parser::expression()
{
	Token t = ts->get();
	switch (t.type) {
		case Token::NAME: {

			Token t2 = ts->get();

			switch (t2.type) {
				case Token::ASSIGN:
				case Token::ADD_ASSIGN:
				case Token::AND_ASSIGN:
				case Token::BITWISE_AND_ASSIGN:
				case Token::BITWISE_NOT_ASSIGN:
				case Token::BITWISE_OR_ASSIGN:
				case Token::BITWISE_XOR_ASSIGN:
				case Token::DIVIDE_ASSIGN:
				case Token::MODULO_ASSIGN:
				case Token::MULTIPLY_ASSIGN:
				case Token::OR_ASSIGN:
				case Token::SHIFT_LEFT_ASSIGN:
				case Token::SHIFT_RIGHT_ASSIGN:
				case Token::SUBTRACT_ASSIGN: {
					return IEvalable::Uptr(new AssignExpression(t2.type, Variable(t.name), expression()));
				}
				default: {
					ts->putback(t2);
					ts->putback(t);
					return relational_expression();
				}
			}

		}

		case Token::VAR: {
			Token t2 = ts->get();

			if (t2.type != Token::NAME) {
				my_error("no variable name found after var keyword");
			}

			Token t3 = ts->get();
			if (t3.type != Token::ASSIGN) {
				ts->putback(t3);
				return IEvalable::Uptr(new DeclareExpression(t2.name, IEvalable::Uptr(new ValueExpression())));
			}

			return IEvalable::Uptr(new DeclareExpression(t2.name, expression()));

		}

		default: {
			ts->putback(t);
			return relational_expression();
		}

	}
}

IEvalable::Uptr Parser::relational_expression()
{
	IEvalable::Uptr lh = additive_expression();

	Token t = ts->get();
	switch (t.type) {
		case Token::LT:
		case Token::LTE:
		case Token::GT:
		case Token::GTE:
		case Token::EQ:
		case Token::INEQ:
		case Token::STRICT_EQ:
		case Token::STRICT_INEQ: {
			return IEvalable::Uptr(new BinaryExpression(t.type, std::move(lh), additive_expression()));
		}
		default: {
			ts->putback(t);
			return lh;
		}

	}

};
IEvalable::Uptr Parser::additive_expression()
{
	IEvalable::Uptr lh = multiplicative_expression();
	while (true) {
		Token t = ts->get();
		switch (t.type) {
		case Token::Type::MINUS:
		case Token::Type::PLUS: {
			lh = std::move(IEvalable::Uptr(new BinaryExpression(t.type, std::move(lh), multiplicative_expression())));
			break;
		}
		default:
			ts->putback(t);
			return lh;
		}
	}
};


IEvalable::Uptr Parser::multiplicative_expression()
{
	IEvalable::Uptr lh = unary_expression();

	Token t = ts->get();
	switch (t.type) {
		case Token::Type::STAR:
		case Token::Type::SLASH:
		case Token::Type::MODULO: {
			IEvalable::Uptr rh = multiplicative_expression();
			return IEvalable::Uptr(new BinaryExpression(t.type, std::move(lh), std::move(rh)));
		}
		default:
			ts->putback(t);
			return lh;
	}

};


IEvalable::Uptr Parser::unary_expression()
{
	Token t = ts->get();
	switch (t.type) {
		case Token::Type::MINUS:
		case Token::Type::PLUS:
		case Token::Type::NOT:
		case Token::Type::BITWISE_NOT: {
			return IEvalable::Uptr(new UnaryExpression(t.type, primary()));
		}
		default: {
			ts->putback(t);

			IEvalable::Uptr p = primary();
			Token t2 = ts->get();

			while (t2.type == Token::OPEN_BRACKET) {
				ts->putback(t2);
				p = std::move(function_call(std::move(p)));
				t2 = ts->get();
			}
			ts->putback(t2);
			return p;
		}
	}
};






IEvalable::Uptr Parser::primary()
{
	Token t = ts->get();

	switch (t.type) {
		case Token::NUMBER: return IEvalable::Uptr(new ValueExpression(Value(t.value)));
		case Token::STRING: return IEvalable::Uptr(new ValueExpression(Value(t.name)));
		case Token::TRUE: return IEvalable::Uptr(new ValueExpression(Value(true)));
		case Token::FALSE: return IEvalable::Uptr(new ValueExpression(Value(false)));
		case Token::NAME: {
			Token t2 = ts->get();
			if (t2.type == Token::DECREMENT || t2.type == Token::INCREMENT) {
				return IEvalable::Uptr(new UnaryAssignExpression(
					   UnaryAssignExpression::POSTFIX, t2.type, Variable(t.name)));
			}
			ts->putback(t2);
			return IEvalable::Uptr(new Variable(t.name));
		}
		case Token::DECREMENT:
		case Token::INCREMENT: {
			Token t2 = ts->get();
			if (t2.type != Token::NAME) {
				ts->putback(t2);
				my_error("can only increment or decrement variables");
			}
			return IEvalable::Uptr(new UnaryAssignExpression(
				   UnaryAssignExpression::PREFIX, t2.type, Variable(t2.name)));
		}
		case Token::PLUS:
		case Token::MINUS:
		case Token::NOT:
		case Token::BITWISE_NOT: return IEvalable::Uptr(new UnaryExpression(t.type, unary_expression()));
		case Token::OPEN_BRACKET: {
			auto e = expression();
			t = ts->get();
			if (t.type != Token::CLOSE_BRACKET) {
				ts->putback(t);
				my_error("missing closing braket");
			}
			return e;
		}
		case Token::FUNCTION: return function();
		default: {
			ts->putback(t);
			my_error("invalid primary :" + string(1, t.type));
			return 0;
		}
	}

}

IEvalable::Uptr Parser::function()
{
	Token t2 = ts->get();
	if (t2.type != Token::OPEN_BRACKET) {
		my_error("expected opening bracket after function keyword");
	}

	vector<Variable> argument_names;

	bool expect_name = false;
	bool end = false;

	while (!end) {
		Token t3 = ts->get();
		switch (t3.type) {
			case Token::NAME: {
				argument_names.push_back(Variable(t3.name));
				Token t4 = ts->get();
				switch (t4.type) {
					case Token::COMMA: expect_name = true; break;
					case Token::CLOSE_BRACKET: end = true; break;
					default: my_error("Expected comma or close brace after identifier");
				}
				break;
			}
			case Token::CLOSE_BRACKET: {
				if (expect_name) {
					my_error("Expected identifier");
				}
				end = true;
				break;
			}
			default: {
				my_error("Could not parse argument list");
			}
		}
	}
	auto fn = Function::Sptr(new Function(argument_names, statement()));
	return IEvalable::Uptr(new FunctionDeclaration(Value(fn)));

}


IEvalable::Uptr Parser::function_call(IEvalable::Uptr exp)
{
	Token t = ts->get();
	if (t.type != Token::OPEN_BRACKET) {
		ts->putback(t);
		return exp;
	}

	vector<IEvalable*> arguments;
	Token t2 = ts->get();

	if (t2.type != Token::CLOSE_BRACKET) {
		ts->putback(t2);

		while (true) {
			arguments.push_back(expression().release());

			Token t3 = ts->get();

			if (t3.type == Token::COMMA) {
			}
			else if (t3.type == Token::CLOSE_BRACKET) {
				break;
			}
			else {
				my_error("expected comma or close bracket after functionCall argument expression");
			}
		}

	}
	return IEvalable::Uptr(new FunctionCall(std::move(exp), arguments));

}