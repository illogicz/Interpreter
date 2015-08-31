#include "stdafx.h"

#include "Program.h"
#include "Value.h"
#include "Scope.h"
#include "Statement.h"
#include "IEvalable.h"
#include "Jump.h"

Jump EmptyStatement::execute(Scope::Sptr scope)  const 
{ 
	return Jump(); 
}

ExpressionStatement::ExpressionStatement(IEvalable::Uptr expression) 
	: expression(move(expression)) {};
Jump ExpressionStatement::execute(Scope::Sptr scope)  const 
{
	expression->evaluate(scope);
	return Jump(Jump::NONE);
};




ConditionalStatement::ConditionalStatement(IEvalable::Uptr c, Statement* s)
	: condition(move(c)), statement(s) {};

ConditionalStatement::ConditionalStatement(IEvalable::Uptr c, Statement* s, Statement* e)
	: condition(move(c)), statement(s), else_statement(e) {};

ConditionalStatement::~ConditionalStatement(){
	delete statement;
	if (else_statement != nullptr)
		delete else_statement;
}
Jump ConditionalStatement::execute(Scope::Sptr scope)  const
{
	if (condition->evaluate(scope))
		return statement->execute(scope);
	else if (else_statement != nullptr)
		return else_statement->execute(scope);
	else
		return Jump(Jump::NONE);
}


WhileStatement::WhileStatement(IEvalable::Uptr c, Statement* s)
	: condition(move(c)), statement(s) {};

WhileStatement::~WhileStatement() {
	delete statement;
}
Jump WhileStatement::execute(Scope::Sptr scope)  const
{
	while (condition->evaluate(scope)) {
		Jump j = statement->execute(scope);
		switch (j.type) {
			case Jump::RETURN:
			case Jump::ERROR: return j;
			case Jump::BREAK: return Jump();
		}
	}
	return Jump();
};


ForStatement::ForStatement(IEvalable::Uptr i, IEvalable::Uptr c, IEvalable::Uptr e, Statement* s)
	: init(move(i)), condition(move(c)), end(move(e)), statement(s) {};

ForStatement::~ForStatement() {
	delete statement;
}
Jump ForStatement::execute(Scope::Sptr scope) const
{
	auto childScope = scope->makeChild();
	init->evaluate(childScope);
	while (condition->evaluate(childScope)) {
		Jump j = statement->execute(childScope);
		switch (j.type) {
			case Jump::RETURN:
			case Jump::ERROR: return j;
			case Jump::BREAK: return Jump();
		}
		end->evaluate(childScope);
	}
	return Jump();
};

TryCatchStatement::TryCatchStatement(Statement* try_s, Statement* catch_s, Variable catch_a)
	: try_s(try_s), catch_s(catch_s), catch_a(catch_a) {};
TryCatchStatement::~TryCatchStatement() {
	delete try_s;
	delete catch_s;
}
Jump TryCatchStatement::execute(Scope::Sptr scope) const
{
	Jump j;
	try {
		j = try_s->execute(scope);
		if (j.type != Jump::ERROR) {
			return j;
		}
	}
	catch (exception e) {
		j = Jump(Jump::ERROR, Value(string(e.what())));
	}

	auto childScope = scope->makeChild() ;
	childScope->define(catch_a, j.value);

	return catch_s->execute(childScope);
}


JumpStatement::JumpStatement(Jump::Type type)
	: type(type) {};
JumpStatement::JumpStatement(Jump::Type type, IEvalable::Uptr e)
	: type(type), expression(std::move(e)) {};
Jump JumpStatement::execute(Scope::Sptr scope) const
{
	if (type == Jump::ERROR) {
		if (expression)
			my_error(string(expression->evaluate(scope)));
		else
			my_error("exception with no value");
	} else {
		if (expression)
			return Jump(type, expression->evaluate(scope));
		else
			return Jump(type);
	}
}

CompoundStatement::~CompoundStatement() {
	while (!statements.empty()) {
		delete statements.back();
		statements.pop_back();
	}
}
void CompoundStatement::add(Statement* s) {
	statements.push_back(s);
}
Jump CompoundStatement::execute(Scope::Sptr scope) const
{
	auto childScope = scope->makeChild();
	for (Statement* s : statements)
	{
		Jump j = s->execute(childScope);
		if (j.type != Jump::NONE) return j;
	}
	return Jump();
}

Program::Program()
{
	scope = make_shared<Scope>();
}

Program::~Program()
{
	scope->dispose();
}

Jump Program::execute() {
	Jump j(Jump::NONE);
	for (Statement* s : statements)
	{
		j = s->execute(scope);
		if (j.type == Jump::RETURN || j.type == Jump::BREAK) {
			cout << "uncaught return or break statement";
			break;
		}
	}
	return j;
}