#pragma once

#include "IEvalable.h"
#include "Value.h"
#include "Token.h"
#include "Utils.h"
#include "Scope.h"


class Variable : public IEvalable
{
public:
	unsigned int id;
	Variable() {};
	Variable(string v);
	Value evaluate(Scope::Sptr scope);
};

class ValueExpression : public IEvalable {
	private:
		Value val;
	public:
		ValueExpression(): val(Value()) {};
		ValueExpression(Value v) : val(v) {};
		Value evaluate(Scope::Sptr scope);
};

class UnaryExpression : public IEvalable {
	private:
		Token::Type op;
		IEvalable::Uptr lh;
	public:
		UnaryExpression(Token::Type op, IEvalable::Uptr lh)
			: op(op), lh(std::move(lh)) {};
		Value evaluate(Scope::Sptr scope);
};

class BinaryExpression : public IEvalable {
	private:
		Token::Type op;
		IEvalable::Uptr lh;
		IEvalable::Uptr rh;
	public:
		BinaryExpression(Token::Type op, IEvalable::Uptr lh, IEvalable::Uptr rh)
			: op(op), lh(std::move(lh)), rh(std::move(rh)) {};
		Value evaluate(Scope::Sptr scope);
};

class AssignExpression : public IEvalable {
	private:
		Token::Type op;
		Variable variable;
		IEvalable::Uptr rh;
	public:
		AssignExpression(Token::Type op, Variable v, IEvalable::Uptr rh)
			: op(op), rh(std::move(rh)), variable(v) {};
		Value evaluate(Scope::Sptr scope);
};

class DeclareExpression : public IEvalable {
	private:
		Variable variable;
		IEvalable::Uptr rh;
	public:
		DeclareExpression(Variable v, IEvalable::Uptr rh)
			: rh(std::move(rh)), variable(v) {};
		Value evaluate(Scope::Sptr scope);
};

class UnaryAssignExpression : public IEvalable {
	private:
		Variable variable;
		Token::Type op;

	public:
		enum Type {
			PREFIX,
			POSTFIX
		};

		Type type;
		UnaryAssignExpression(Type type, Token::Type op, Variable v)
			: type(type), variable(v), op(op) {};
		Value evaluate(Scope::Sptr scope);

};

class FunctionDeclaration : public IEvalable {
	private:
		Value val;
	public:
		FunctionDeclaration(Value v) : val(v) {};
		Value evaluate(Scope::Sptr scope);
};


class FunctionCall : public IEvalable {
	private:
		IEvalable::Uptr lh;
		vector<IEvalable*> arguments;
	public:
		FunctionCall(IEvalable::Uptr lh, vector<IEvalable*> arguments)
			: lh(std::move(lh)), arguments(arguments) {
		};
		~FunctionCall();
		Value evaluate(Scope::Sptr scope);
};


