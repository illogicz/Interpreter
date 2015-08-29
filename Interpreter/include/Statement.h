#pragma once

#include "IEvalable.h"
#include "Expression.h"
#include "Scope.h"


class Jump;

class Statement {
	public:
		virtual ~Statement() {};
		virtual Jump execute(Scope::Sptr scope) = 0;

};

class EmptyStatement : public Statement {
	Jump execute(Scope::Sptr scope);
};

class ConditionalStatement : public Statement {
	public:
		ConditionalStatement(IEvalable::Uptr c, Statement* s);
		ConditionalStatement(IEvalable::Uptr c, Statement* s, Statement* e);
		~ConditionalStatement();
		Jump execute(Scope::Sptr scope);
	private:
		IEvalable::Uptr condition;
		Statement* statement;
		Statement* else_statement;
};

class ReturnStatement : public Statement {
	public:
		IEvalable::Uptr expression;
		ReturnStatement(IEvalable::Uptr expression);
		Jump execute(Scope::Sptr scope);
};

class ExpressionStatement : public Statement {
	public:
		IEvalable::Uptr expression;
		ExpressionStatement(IEvalable::Uptr expression);
		Jump execute(Scope::Sptr scope);
};

class CompoundStatement : public Statement {
	protected:
		vector<Statement*> statements;
	public:
		~CompoundStatement();
		void add(Statement* s);
		Jump execute(Scope::Sptr scope);

};
