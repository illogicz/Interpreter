#include "stdafx.h"

#include "Value.h"
#include "Jump.h"
#include "Expression.h"
#include "Scope.h"
#include "Function.h"
#include "VariableMap.h"


Variable::Variable(string name):
	id(VariableMap::get_id(name)) {};

Value Variable::operator()(Scope::Sptr scope) {
	return scope->get(*this);
}

Value ValueExpression::operator()(Scope::Sptr scope) {
	return val;
};

Value UnaryExpression::operator()(Scope::Sptr scope) {
	Value v = (*lh)(scope);
	switch (op) {
		case Token::PLUS: return v;
		case Token::MINUS: return -v;
		case Token::NOT: return !v;
		case Token::TYPEOF:return v.typeof();
		default: {
			error("Unimplemented unary expression");
			return v;
		}
	}
};
 
Value BinaryExpression::operator()(Scope::Sptr scope) {
	Value l = (*lh)(scope);
	Value r = (*rh)(scope);
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
			error("Unimplemented binary expression");
		}
	}
	return Value();
};

Value DeclareExpression::operator()(Scope::Sptr scope) {
	Value value = (*rh)(scope);
	scope->define(variable, value);
	return value;
};

Value AssignExpression::operator()(Scope::Sptr scope) {
	Value l = variable(scope);
	Value r = (*rh)(scope);
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
			error("Unimplemented assign expression");
		}
	}
	
	scope->set(variable, l);
	return l;
};

Value UnaryAssignExpression::operator()(Scope::Sptr scope) {
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
Value FunctionCall::operator()(Scope::Sptr scope) {

	vector<Value> argument_values;
	for (auto& arg : arguments) {
		argument_values.push_back((*arg)(scope));
	}
	return (*lh)(scope)(argument_values);

};

Value FunctionDeclaration::operator()(Scope::Sptr scope) {
	return Value(func, scope);
};


Value NamedFunctionDeclaration::operator()(Scope::Sptr scope)
{
	Value v = FunctionDeclaration::operator()(scope);
	scope->define(var, v);
	return v;
}
