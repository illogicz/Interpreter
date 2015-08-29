#include "stdafx.h"

#include "Value.h"
#include "Scope.h"
#include "Utils.h"
#include "Function.h"
#include "Statement.h"

Value::Value() : Value(0.0) {};
Value::Value(Type t)   : type(t) {};
Value::Value(double v) : type(Value::Type::NUMBER),  num(v) {};
Value::Value(string v) : type(Value::Type::STRING),  str(v) {};
Value::Value(bool v) : type(Value::Type::BOOLEAN), boolean(v) {};
Value::Value(Function::Sptr v) : type(Value::Type::FUNCTION), func(v) {};

void Value::print()
{
	switch (type) {
		case Type::NUMBER:    cout << num;					return;
		case Type::STRING:    cout << "\"" << str << "\"";	return;
		case Type::BOOLEAN:   cout << boolean;				return;
		case Type::FUNCTION:  cout << "function";			return;
		case Type::UNDEFINED: cout << "undefined";			return;
	}
}


Value& Value::operator+=(const Value& rhs)
{
	if (type == rhs.type) {
		switch (type) {
			case Value::NUMBER: num += rhs.num; break;
			case Value::STRING: str += rhs.str; break;
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
	if (type == Type::NUMBER && rhs.type == Type::NUMBER) {
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
	if (type == Type::NUMBER && rhs.type == Type::NUMBER) {
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
	if (type == Type::NUMBER && rhs.type == Type::NUMBER) {
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
