#include "stdafx.h"
#include "Utils.h"
#include "Value.h"
#include "Jump.h"
#include "Expression.h"
#include "Scope.h"
#include "Function.h"
#include "VariableMap.h"


Variable::Variable(string name):
	id(VariableMap::get_id(name)) {};

Value Variable::evaluate(Scope::Sptr scope) {
	return scope->get(*this);
}

Value ValueExpression::evaluate(Scope::Sptr scope) {
	return val;
};

Value UnaryExpression::evaluate(Scope::Sptr scope) {
	Value v = lh->evaluate(scope);
	switch (op) {
		case Token::PLUS: break;
		case Token::MINUS: v = -v; break;
		default: {
			my_error("Unimplemented unary expression");
		}
	}
	return v;
};

Value BinaryExpression::evaluate(Scope::Sptr scope) {
	Value l = lh->evaluate(scope);
	Value r = rh->evaluate(scope);
	switch (op) {
		case Token::PLUS: return l + r;
		case Token::MINUS: return l - r;
		case Token::STAR: return l * r;
		case Token::SLASH: return l / r;
		default: {
			my_error("Unimplemented binary expression");
		}
	}
	return Value();
};

Value DeclareExpression::evaluate(Scope::Sptr scope) {
	Value value = rh->evaluate(scope);
	scope->define(variable, value);
	return value;
};

Value AssignExpression::evaluate(Scope::Sptr scope) {
	Value l = variable.evaluate(scope);
	Value r = rh->evaluate(scope);
	switch (op) {
		case Token::ASSIGN: l = r;  break;
		case Token::ADD_ASSIGN: l += r; break;
		case Token::SUBTRACT_ASSIGN: l -= r; break;
		case Token::MULTIPLY_ASSIGN: l *= r; break;
		case Token::DIVIDE_ASSIGN: l /= r; break;
		default: {
			my_error("Unimplemented assign expression");
		}
	}
	
	scope->set(variable, l);
	return l;
};

Value UnaryAssignExpression::evaluate(Scope::Sptr scope) {
	Value value = scope->get(variable);
	//check if number, post or
	scope->set(variable, value);
	return value;
};


FunctionCall::~FunctionCall() {
	while (!arguments.empty()) {
		delete arguments.back();
		arguments.pop_back();
	}
}
Value FunctionCall::evaluate(Scope::Sptr scope) {

	Value v = lh->evaluate(scope);
	if (v.type != Value::FUNCTION) {
		my_error("Not a function");
	}

	//Function::Sptr f = v.func;
	vector<Value> argument_values;
	for (size_t i = 0; i < arguments.size(); i++) {
		argument_values.push_back(arguments[i]->evaluate(scope));
	}
	return v.func->execute(v.closure, argument_values).value;

};

Value FunctionDeclaration::evaluate(Scope::Sptr scope) {
	Value cpy = val;
	cpy.closure = scope;
	return cpy;
};
