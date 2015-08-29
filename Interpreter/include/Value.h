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

		Value& operator--();
		Value& operator++();
		Value operator+();
		Value operator-();
		Value operator!();

		bool operator==(const Value& value) const;
		bool operator!=(const Value& value) const;
		bool equality(const Value& value) const;
		bool inequality(const Value& value) const;
		bool operator<(const Value& value) const;
		bool operator<=(const Value& value) const;
		bool operator>(const Value& value) const;
		bool operator>=(const Value& value) const;

		Value operator()(const vector<Value>& args);

		operator bool() const;
		operator string() const;
		operator double() const;
};


