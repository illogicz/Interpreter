#pragma once

#include "IEvalable.h"
#include "Expression.h"
#include "Scope.h"
#include "Jump.h"

class Statement {
	public:
		virtual ~Statement() {};
		virtual Jump execute(Scope::Sptr scope) const = 0;

};


class EmptyStatement : public Statement {
	Jump execute(Scope::Sptr scope) const;
};


class ConditionalStatement : public Statement {
	public:
		ConditionalStatement(IEvalable::Uptr c, Statement* s);
		ConditionalStatement(IEvalable::Uptr c, Statement* s, Statement* e);
		~ConditionalStatement();
		Jump execute(Scope::Sptr scope) const;
	private:
		IEvalable::Uptr condition;
		Statement* statement;
		Statement* else_statement;
};


class WhileStatement : public Statement {
public:
	WhileStatement(IEvalable::Uptr c, Statement* s);
	~WhileStatement();
	Jump execute(Scope::Sptr scope) const;
private:
	IEvalable::Uptr condition;
	Statement* statement;
};


class ForStatement : public Statement {
public:
	ForStatement(IEvalable::Uptr i, IEvalable::Uptr c, IEvalable::Uptr e, Statement* s);
	~ForStatement();
	Jump execute(Scope::Sptr scope) const;
private:
	IEvalable::Uptr init;
	IEvalable::Uptr condition;
	IEvalable::Uptr end;
	Statement* statement;
};


class TryCatchStatement : public Statement {
public:
	TryCatchStatement(Statement* try_s, Statement* catch_s, Variable catch_a);
	~TryCatchStatement();
	Jump execute(Scope::Sptr scope) const;
private:
	Statement* try_s;
	Statement* catch_s;
	Variable catch_a;
};


class JumpStatement : public Statement {
private:
	Jump::Type type;
	IEvalable::Uptr expression;
public:
	JumpStatement(Jump::Type type);
	JumpStatement(Jump::Type type, IEvalable::Uptr e);
	Jump execute(Scope::Sptr scope)  const;
};


class ExpressionStatement : public Statement {
	public:
		IEvalable::Uptr expression;
		ExpressionStatement(IEvalable::Uptr expression);
		Jump execute(Scope::Sptr scope) const;
};


class CompoundStatement : public Statement {
	protected:
		vector<Statement*> statements;
	public:
		~CompoundStatement();
		void add(Statement* s);
		Jump execute(Scope::Sptr scope) const;
};
