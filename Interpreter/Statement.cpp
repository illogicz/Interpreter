#include "stdafx.h"

#include "Program.h"
#include "Value.h"
#include "Scope.h"
#include "Statement.h"
#include "IEvalable.h"
#include "Jump.h"
#include "Utils.h"

Jump::Jump(Type type)
	: type(type) {};
Jump::Jump(Value value)
	: type(RETURN), value(value) {};


ExpressionStatement::ExpressionStatement(IEvalable::Uptr expression) 
	: expression(std::move(expression)) {};
Jump ExpressionStatement::execute(Scope::Sptr scope) {
	expression->evaluate(scope);
	return Jump(Jump::NONE);
};

ConditionalStatement::ConditionalStatement(IEvalable::Uptr c, Statement* s)
	: condition(std::move(c)), statement(s) {};
ConditionalStatement::ConditionalStatement(IEvalable::Uptr c, Statement* s, Statement* e)
	: condition(std::move(c)), statement(s), else_statement(e) {};
ConditionalStatement::~ConditionalStatement()
{
	delete statement;
	if (else_statement != nullptr)
		delete else_statement;
}
Jump ConditionalStatement::execute(Scope::Sptr scope)
{
	if (condition->evaluate(scope))
		return statement->execute(scope);
	else if (else_statement != nullptr)
		return else_statement->execute(scope);
	else
		return Jump(Jump::NONE);
}


ReturnStatement::ReturnStatement(IEvalable::Uptr expression)
	: expression(std::move(expression)) {};
Jump ReturnStatement::execute(Scope::Sptr scope) {
	return Jump(expression->evaluate(scope));
};


CompoundStatement::~CompoundStatement() {
	while (!statements.empty()) {
		delete statements.back();
		statements.pop_back();
	}
}
void CompoundStatement::add(Statement* s) {
	statements.push_back(s);
}
Jump CompoundStatement::execute(Scope::Sptr scope) {
	Scope::Sptr childScope = make_shared<Scope>(scope);
	Jump j(Jump::NONE);
	for (Statement* s : statements)
	{
		j = s->execute(childScope);
		if (j.type == Jump::RETURN || j.type == Jump::BREAK) {
			break;
		}
	}
	return j;
}

Program::Program()
{
	scope = make_shared<Scope>();
}

Program::~Program()
{
	debug("Program destructor");
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