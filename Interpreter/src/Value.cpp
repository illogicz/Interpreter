#include "stdafx.h"

#include "Value.h"
#include "Scope.h"
#include "Utils.h"
#include "Function.h"
#include "Statement.h"

Value::Value() : type(UNDEFINED) {};
Value::Value(Type t)   : type(t) {};
Value::Value(double v) : type(NUMBER),  num(v) {};
Value::Value(string v) : type(STRING),  str(v) {};
Value::Value(bool v) : type(BOOLEAN), boolean(v) {};
Value::Value(Function::Sptr v) : type(FUNCTION), func(v) {};

void Value::print()
{
	switch (type) {
		case NUMBER:    cout << num;					return;
		case STRING:    cout << "\"" << str << "\"";	return;
		case BOOLEAN:   cout << boolean;				return;
		case FUNCTION:  cout << "function";			return;
		case UNDEFINED: cout << "undefined";			return;
	}
}


Value& Value::operator+=(const Value& rhs)
{
	if (type == rhs.type) {
		switch (type) {
			case NUMBER: num += rhs.num; break;
			case STRING: str += rhs.str; break;
			default: {
				my_error("can only add numbers and strings");
			}
		}
	}
	else {

	}
	return *this;
}

Value Value::operator+()
{
	if (type != NUMBER) {
		my_error("Unary + can only operate on numbers");
	}
	return *this;
}


Value operator+(Value lhs, const Value& rhs)
{
	lhs += rhs;
	return lhs;
}



Value& Value::operator-=(const Value& rhs)
{
	if (type == NUMBER && rhs.type == NUMBER) {
		num -= rhs.num;
	}
	return *this;
}

Value Value::operator-()
{
	if (type != NUMBER) {
		my_error("Unary - can only operate on numbers");
	}
	return Value(-num);
}

Value operator-(Value lhs, const Value& rhs)
{
	lhs -= rhs;
	return lhs;
}

Value& Value::operator*=(const Value& rhs)
{
	if (type == NUMBER && rhs.type == NUMBER) {
		num *= rhs.num;
	}
	else {
		my_error("* can only operate on numbers");
	}
	return *this;
}

Value operator*(Value lhs, const Value& rhs)
{
	lhs *= rhs;
	return lhs;
}

Value& Value::operator/=(const Value& rhs)
{
	if (type == NUMBER && rhs.type == NUMBER) {
		if (rhs.num == 0) {
			my_error("divide by 0");
		}
		num /= rhs.num;
	}
	else {
		my_error("/ can only operate on numbers");
	}
	return *this;
}

Value operator/(Value lhs, const Value& rhs)
{
	lhs /= rhs;
	return lhs;
}

Value::operator bool() const
{
	switch (type) {
		case NUMBER: {
			debug("number -> bool:" + to_string(num != 0));
			return num != 0;
		}
		case STRING: return str != "";
		case BOOLEAN: return boolean;
		case FUNCTION: return true;
		case UNDEFINED: return false;
		default: return false;
	}
}
