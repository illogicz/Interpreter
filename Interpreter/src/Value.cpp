#include "stdafx.h"

#include "Value.h"
#include "Scope.h"
#include "Function.h"
#include "Statement.h"

Value::Value() : type(UNDEFINED) {};
Value::Value(Type t)   : type(t) {};
Value::Value(double v) : type(NUMBER),  num(v) {};
Value::Value(string v) : type(STRING),  str(v) {};
Value::Value(bool v) : type(BOOLEAN), bln(v) {};
Value::Value(Function::Sptr v) : type(FUNCTION), func(v) {};

Value operator+(Value lhs, const Value& rhs)
{
	lhs += rhs;
	return lhs;
}



// strict;
bool Value::operator==(const Value& rhs) const
{
	if (type != rhs.type) return false;
	switch (type) {
		case NUMBER:    return num == rhs.num;
		case STRING:	return str == rhs.str;
		case BOOLEAN:   return bln == rhs.bln;
		case FUNCTION:  return func == rhs.func;
		case UNDEFINED: return true;
		default: return false;
	}
}

bool Value::operator!=(const Value& value) const
{
	return !(*this == value);
}

bool Value::equality(const Value& rhs) const
{
	if (type == rhs.type) return *this == rhs;

	switch (type) {
		case BOOLEAN:   return bln == bool(rhs);
		case FUNCTION:  return false;
		case UNDEFINED: return !(rhs);
		case STRING: return rhs.equality(*this);
		case NUMBER:
			switch (rhs.type) {
				case BOOLEAN:
				case FUNCTION:
				case UNDEFINED: return rhs.equality(*this);
				case STRING: {
					try {
						return double(rhs) == num;
					} catch (exception e) {
						my_error("warning: failed casting string to double for comparison");
					}
					return false;
				}
			}
	}

	my_error("internal error: undefined/impossible comparison");
	return false;
}

bool Value::inequality(const Value& rhs) const
{
	return !this->equality(rhs);
}


bool Value::operator<(const Value& value) const {
	return double(*this) < double(value);
}
bool Value::operator<=(const Value& value) const {
	return double(*this) <= double(value);
}
bool Value::operator>(const Value& value) const {
	return double(*this) > double(value);
}
bool Value::operator>=(const Value& value) const {
	return double(*this) >= double(value);
}


Value& Value::operator+=(const Value& rhs)
{
	if (type == STRING || rhs.type == STRING || type == FUNCTION || rhs.type == FUNCTION) {
		str = string(*this) + string(rhs);
		type = STRING;
	}
	else {
		num = double(*this) + double(rhs);
		type = NUMBER;
	}
	return *this;
}

Value& Value::operator-=(const Value& rhs)
{
	if (type == NUMBER && rhs.type == NUMBER) {
		num -= rhs.num;
	}
	return *this;
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
			num = numeric_limits<int>::max();
		}
		else {
			num /= rhs.num;
		}
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

Value& Value::operator++()
{
	num = double(*this) + 1;
	type = NUMBER;
	return *this;
}

Value& Value::operator--()
{
	num = double(*this) - 1;
	type = NUMBER;
	return *this;
}

Value Value::operator+()
{
	return Value(double(*this));
}

Value Value::operator-()
{
	return Value(-double(*this));
}

Value Value::operator!()
{
	return Value(!bool(*this));
}


Value Value::operator()(const vector<Value>& args)
{
	if (type != Value::FUNCTION) {
		my_error("Not a function");
	}
	return func->execute(closure, args).value;
}


Value::operator bool() const
{
	switch (type) {
		case NUMBER: return num != 0;
		case STRING: return str != "";
		case BOOLEAN: return bln;
		case FUNCTION: return true;
		default: return false;
	}
}


Value::operator string() const
{
	switch (type) {
		case NUMBER:    return num == int(num) ? to_string(int(num)) : to_string(num);
		case STRING:    return str;
		case BOOLEAN:   return (bln ? "true" : "false");
		case FUNCTION:  return "function";
		case UNDEFINED: return "undefined";
		default: return "";
	}
}

Value::operator double() const
{
	switch (type) {
		case NUMBER: return num;
		case STRING: return stod(str);
		case BOOLEAN: return bln ? 1 : 0;
		case FUNCTION: return 1;
		default: return 0;
	}
}
