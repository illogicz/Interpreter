#include "stdafx.h"
#include "Parser.h"
#include "Expression.h"
#include "TokenStream.h"
#include "Value.h"
#include "Function.h"
#include "Program.h"
#include "Jump.h"

Parser::Parser() {};


Program* Parser::parse(TokenStream* token_stream) {
	ts = token_stream;
	try {
		return new Program(statement_l());
	}
	catch (exception e){
		debug("Parse Error: " + string(e.what()));
		return new Program(new EmptyStatement);
	}
};

Statement* Parser::statement()
{
	Token t = ts->get();

	switch (t.type) {
		case Token::EOL:		return new EmptyStatement;
		case Token::FOR:		return for_statement();
		case Token::WHILE:		return while_statement();
		case Token::IF:			return if_statement();
		case Token::RETURN:		return jump_statement(Jump::RETURN);
		case Token::BREAK:		return jump_statement(Jump::BREAK);
		case Token::CONTINUE:	return jump_statement(Jump::CONTINUE);
		case Token::THROW:		return jump_statement(Jump::ERROR);
		case Token::TRY:		return try_statement();
		case Token::OPEN_BRACE: return block();
		default: {
			ts->putback(t);
			return new ExpressionStatement(expression()); // expression_statement
		}
	}

};

Statement* Parser::block()
{
	Statement* s = (Statement*)statement_l();
	if (ts->get().type != Token::CLOSE_BRACE) {
		error("no closing brace found");
	}
	return s;
};

Statement* Parser::statement_l()
{
	auto* cs = new CompoundStatement();
	Token t = ts->get();
	while (true) {
		switch (t.type) {
			case Token::CLOSE_BRACE: 
			case Token::EOFILE: {
				ts->putback(t);
				return cs;
			}
			case Token::EOL: break;
			default: {
				ts->putback(t);
				cs->add(statement());
			}
		}
		t = ts->get();
	}
};


Statement* Parser::try_statement()
{
	Statement* try_s = statement();
	if (ts->get().type != Token::CATCH) {
		error("expected catch after try");
	}
	if (ts->get().type != Token::OPEN_BRACKET) {
		error("expected catch after try");
	}
	Token t = ts->get();
	if (t.type != Token::IDENTIFIER) {
		error("expected argument for catch");
	}
	Variable catch_a = Variable(t.name);
	if (ts->get().type != Token::CLOSE_BRACKET) {
		error("expected close bracket after catch argument");
	}
	Statement* catch_s = statement();

	return new TryCatchStatement(try_s, catch_s, catch_a);

};

Statement* Parser::jump_statement(Jump::Type type)
{
	return new JumpStatement(type, expression_or_empty());
};

Statement* Parser::for_statement()
{
	Token t = ts->get();
	if (t.type != Token::OPEN_BRACKET) {
		error("expected open bracket after conditional keyword");
	}

	IEvalable::Uptr i = for_expression(Value());
	IEvalable::Uptr c = for_expression(Value(true));
	IEvalable::Uptr e = for_expression(Value());

	return new ForStatement(move(i), move(c), move(e), statement());


};

IEvalable::Uptr Parser::for_expression(Value value)
{
	Token t = ts->get();
	switch (t.type) {
		case Token::CLOSE_BRACKET:
		case Token::EOL: return move(IEvalable::Uptr(new ValueExpression(value)));
		default: {
			ts->putback(t);
			auto e = expression();
			t = ts->get();
			switch (t.type) {
				case Token::CLOSE_BRACKET:
				case Token::EOL: return move(e);
				default: error("error parsing optional expression");
			}
		}

	}
};

IEvalable::Uptr Parser::expression_or_empty()
{
	Token t = ts->get();
	switch (t.type) {
		case Token::EOL: return move(IEvalable::Uptr(new ValueExpression(Value())));
		default: {
			ts->putback(t);
			return expression();
		}
	}
};

Statement* Parser::while_statement()
{
	auto c = condition();
	Statement* s = statement();
	return new WhileStatement(move(c), s);
};

Statement* Parser::if_statement()
{

	auto c = condition();
	Statement* s = statement();

	Token t = ts->get();
	if (t.type != Token::ELSE) {
		ts->putback(t);
		return new ConditionalStatement(move(c), s);
	}

	return new ConditionalStatement(move(c), s, statement());

};

IEvalable::Uptr Parser::condition()
{
	Token t = ts->get();
	if (t.type != Token::OPEN_BRACKET) {
		error("expected open bracket after conditional keyword");
	}

	auto c = expression();

	t = ts->get();
	if (t.type != Token::CLOSE_BRACKET) {
		error("expected closing bracket after condition");
	}

	return c;
};



IEvalable::Uptr Parser::expression()
{
	Token t = ts->get();
	switch (t.type) {
		case Token::IDENTIFIER: {

			Token t2 = ts->get();

			switch (t2.type) {
				case Token::ASSIGN:
				case Token::ADD_ASSIGN:
				case Token::BITWISE_AND_ASSIGN:
				case Token::BITWISE_NOT_ASSIGN:
				case Token::BITWISE_OR_ASSIGN:
				case Token::BITWISE_XOR_ASSIGN:
				case Token::DIVIDE_ASSIGN:
				case Token::MODULO_ASSIGN:
				case Token::MULTIPLY_ASSIGN:
				case Token::SHIFT_LEFT_ASSIGN:
				case Token::SHIFT_RIGHT_ASSIGN:
				case Token::SUBTRACT_ASSIGN: {
					return IEvalable::Uptr(new AssignExpression(t2.type, Variable(t.name), expression()));
				}
				default: {
					ts->putback(t2);
					ts->putback(t);
					return logical_or_expression();
				}
			}

		}

		case Token::VAR: {
			Token t2 = ts->get();

			if (t2.type != Token::IDENTIFIER) {
				error("no variable name found after var keyword");
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
			return logical_or_expression();
		}

	}
};

IEvalable::Uptr Parser::logical_or_expression()
{
	IEvalable::Uptr lh = logical_and_expression();
	while (true) {
		Token t = ts->get();
		switch (t.type) {
			case Token::Type::OR: {
				lh = IEvalable::Uptr(new BinaryExpression(t.type, move(lh), logical_and_expression()));
				break;
			}
			default:
				ts->putback(t);
				return lh;
		}
	}
};

IEvalable::Uptr Parser::logical_and_expression()
{
	IEvalable::Uptr lh = equality_expression();
	while (true) {
		Token t = ts->get();
		switch (t.type) {
			case Token::Type::AND: {
				lh = IEvalable::Uptr(new BinaryExpression(t.type, move(lh), equality_expression()));
				break;
			}
			default:
				ts->putback(t);
				return lh;
		}
	}
};

IEvalable::Uptr Parser::equality_expression()
{
	IEvalable::Uptr lh = relational_expression();

	Token t = ts->get();
	switch (t.type) {
		case Token::EQ:
		case Token::INEQ:
		case Token::STRICT_EQ:
		case Token::STRICT_INEQ: {
			return IEvalable::Uptr(new BinaryExpression(t.type, move(lh), relational_expression()));
		}
		default: {
			ts->putback(t);
			return lh;
		}

	}

};

IEvalable::Uptr Parser::relational_expression()
{
	IEvalable::Uptr lh = shift_expression();

	Token t = ts->get();
	switch (t.type) {
		case Token::LT:
		case Token::LTE:
		case Token::GT:
		case Token::GTE: {
			return IEvalable::Uptr(new BinaryExpression(t.type, move(lh), shift_expression()));
		}
		default: {
			ts->putback(t);
			return lh;
		}
		
	}

};


IEvalable::Uptr Parser::shift_expression()
{
	IEvalable::Uptr lh = additive_expression();
	while (true) {
		Token t = ts->get();
		switch (t.type) {
			case Token::Type::SHIFT_LEFT:
			case Token::Type::SHIFT_RIGHT: {
				lh = IEvalable::Uptr(new BinaryExpression(t.type, move(lh), additive_expression()));
				break;
			}
			default:
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
			lh = IEvalable::Uptr(new BinaryExpression(t.type, move(lh), multiplicative_expression()));
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
		case Token::Type::DIVIDE:
		case Token::Type::MODULO: {
			IEvalable::Uptr rh = multiplicative_expression();
			return IEvalable::Uptr(new BinaryExpression(t.type, move(lh), move(rh)));
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
		case Token::MINUS:
		case Token::PLUS:
		case Token::NOT:
		case Token::BITWISE_NOT:
		case Token::TYPEOF: return IEvalable::Uptr(new UnaryExpression(t.type, unary_expression()));
		default: {
			ts->putback(t);

			IEvalable::Uptr p = primary();
			Token t2 = ts->get();

			while (t2.type == Token::OPEN_BRACKET) {
				ts->putback(t2);
				p = move(function_call(move(p)));
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
		case Token::NUMBER:    return value(Value(t.value));
		case Token::STRING:    return value(Value(t.name));
		case Token::TRUE:	   return value(Value(true));
		case Token::FALSE:	   return value(Value(false));
		case Token::UNDEFINED: return value(Value());
		case Token::IDENTIFIER: {
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
			if (t2.type != Token::IDENTIFIER) {
				ts->putback(t2);
				error("can only increment or decrement variables");
			}
			return IEvalable::Uptr(new UnaryAssignExpression(
				   UnaryAssignExpression::PREFIX, t.type, Variable(t2.name)));
		}
		case Token::OPEN_BRACKET: {
			auto e = expression();
			t = ts->get();
			if (t.type != Token::CLOSE_BRACKET) {
				ts->putback(t);
				error("missing closing braket");
			}
			return e;
		}
		case Token::FUNCTION: return function();
		default: {
			ts->putback(t);
			error("invalid primary :" + string(1, t.type));
			return 0;
		}
	}

};

IEvalable::Uptr Parser::value(Value v)
{
	return IEvalable::Uptr(new ValueExpression(v));
};

IEvalable::Uptr Parser::function()
{
	Token t2 = ts->get();
	string name = "";
	if (t2.type == Token::IDENTIFIER) {
		name = t2.name;
		t2 = ts->get();
	}

	if (t2.type != Token::OPEN_BRACKET) {
		error("expected opening bracket after function keyword");
	}

	vector<Variable> argument_names;

	bool expect_name = false;
	bool end = false;

	while (!end) {
		Token t3 = ts->get();
		switch (t3.type) {
			case Token::IDENTIFIER: {
				argument_names.push_back(Variable(t3.name));
				Token t4 = ts->get();
				switch (t4.type) {
					case Token::COMMA: expect_name = true; break;
					case Token::CLOSE_BRACKET: end = true; break;
					default: error("Expected comma or close brace after identifier");
				}
				break;
			}
			case Token::CLOSE_BRACKET: {
				if (expect_name) {
					error("Expected identifier");
				}
				end = true;
				break;
			}
			default: {
				error("Could not parse argument list");
			}
		}
	}
	auto fn = IFunction::Sptr(new Function(argument_names, *statement()));

	if (name != "")
		return IEvalable::Uptr(new NamedFunctionDeclaration(fn, Variable(name)));
	else
		return IEvalable::Uptr(new FunctionDeclaration(fn));
};


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
				error("expected comma or close bracket after functionCall argument expression");
			}
		}

	}
	return IEvalable::Uptr(new FunctionCall(move(exp), arguments));

};
