#include "stdafx.h"

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
		case Token::PLUS: return v;
		case Token::MINUS: return -v;
		case Token::NOT: return !v;
		case Token::TYPEOF:return v.typeof();
		default: {
			my_error("Unimplemented unary expression");
			return v;
		}
	}

};
 
Value BinaryExpression::evaluate(Scope::Sptr scope) {
	Value l = lh->evaluate(scope);
	Value r = rh->evaluate(scope);
	switch (op) {
		case Token::PLUS:		return l + r;
		case Token::MINUS:		return l - r;
		case Token::STAR:		return l * r;
		case Token::DIVIDE:		return l / r;
		case Token::STRICT_EQ:	return l == r;
		case Token::STRICT_INEQ:return l != r;
		case Token::EQ:			return l.equality(r);
		case Token::INEQ:		return l.inequality(r);
		case Token::GT:			return l > r;
		case Token::GTE:		return l >= r;
		case Token::LT:			return l < r;
		case Token::LTE:		return l <= r;
		case Token::MODULO:		return l % r;
		case Token::SHIFT_LEFT:	return l << r;
		case Token::SHIFT_RIGHT:return l >> r;
		case Token::OR:			return l || r;
		case Token::AND:		return l && r;
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
		case Token::ASSIGN:				l = r;   break;
		case Token::ADD_ASSIGN:			l += r;  break;
		case Token::SUBTRACT_ASSIGN:	l -= r;  break;
		case Token::MULTIPLY_ASSIGN:	l *= r;  break;
		case Token::DIVIDE_ASSIGN:		l /= r;  break;
		case Token::MODULO_ASSIGN:		l %= r;  break;
		case Token::SHIFT_LEFT_ASSIGN:	l <<= r; break;
		case Token::SHIFT_RIGHT_ASSIGN:	l >>= r; break;
		default: {
			my_error("Unimplemented assign expression");
		}
	}
	
	scope->set(variable, l);
	return l;
};

Value UnaryAssignExpression::evaluate(Scope::Sptr scope) {
	Value oldvalue = scope->get(variable);
	Value newvalue = oldvalue;
	switch (op) {
		case Token::INCREMENT: ++newvalue; break;
		case Token::DECREMENT: --newvalue; break;
	}
	scope->set(variable, newvalue);
	return type == POSTFIX ? oldvalue : newvalue;
};


FunctionCall::~FunctionCall() {
	while (!arguments.empty()) {
		delete arguments.back();
		arguments.pop_back();
	}
}
Value FunctionCall::evaluate(Scope::Sptr scope) {

	vector<Value> argument_values;
	for (auto& arg : arguments) {
		argument_values.push_back(arg->evaluate(scope));
	}
	return lh->evaluate(scope)(argument_values);

};

Value FunctionDeclaration::evaluate(Scope::Sptr scope) {
	Value cpy = val;
	cpy.closure = scope;
	return cpy;
};


Value NamedFunctionDeclaration::evaluate(Scope::Sptr scope)
{
	Value v = FunctionDeclaration::evaluate(scope);
	scope->define(var, v);
	return v;
}
