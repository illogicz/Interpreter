#pragma once

#include "Function.h"
#include "Scope.h"

class Value {
	public:
		enum Type {
			NUMBER,
			STRING,
			BOOLEAN,
			// NaN, // ???
			// OBJECT,
			FUNCTION,
			UNDEFINED
		};

		Type type;
		double num;
		string str;
		bool bln;
		Function::Sptr func;
		Scope::Sptr closure;

		Value();
		Value(Type t);
		Value(double v);
		Value(string v);
		Value(bool v);
		Value(Function::Sptr v);

		Value& operator+=(const Value& rhs);
		friend Value operator+(Value lhs, const Value& rhs);

		Value& operator-=(const Value& rhs);
		friend Value operator-(Value lhs, const Value& rhs);

		Value& operator*=(const Value& rhs);
		friend Value operator*(Value lhs, const Value& rhs);

		Value& operator/=(const Value& rhs);
		friend Value operator/(Value lhs, const Value& rhs);

		Value& operator%=(const Value& rhs);
		friend Value operator%(Value lhs, const Value& rhs);

		Value& operator<<=(const Value& rhs);
		friend Value operator<<(Value lhs, const Value& rhs);

		Value& operator>>=(const Value& rhs);
		friend Value operator>>(Value lhs, const Value& rhs);

		friend Value operator||(Value lhs, const Value& rhs);
		friend Value operator&&(Value lhs, const Value& rhs);

		Value& operator--();
		Value& operator++();
		Value operator+() const;
		Value operator-() const;
		Value operator!() const;

		bool equality    (const Value& value) const;
		bool inequality  (const Value& value) const;
		bool operator == (const Value& value) const;
		bool operator != (const Value& value) const;
		bool operator <  (const Value& value) const;
		bool operator <= (const Value& value) const;
		bool operator >  (const Value& value) const;
		bool operator >= (const Value& value) const;

		Value operator()(const vector<Value>& args) const;

		inline operator bool() const;
		inline operator string() const;
		inline operator double() const;
		inline operator int() const;

		Value typeof() const;
};


