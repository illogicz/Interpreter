#pragma once
#include "Utils.h"
#include "Function.h"
#include "Scope.h"

class Value {
	public:
		enum Type {
			NUMBER,
			STRING,
			BOOLEAN,
			//OBJECT,
			//ARRAY,
			FUNCTION,
			UNDEFINED
		};

		Type type;
		double num;
		string str;
		bool boolean;
		Function::Sptr func;
		Scope::Sptr closure;

		Value();
		Value(Type t);
		Value(double v);
		Value(string v);
		Value(bool v);
		Value(Function::Sptr v);
		void print();


		Value& operator+=(const Value& rhs);
		Value operator+();
		friend Value operator+(Value lhs, const Value& rhs);

		Value& operator-=(const Value& rhs);
		Value operator-();
		friend Value operator-(Value lhs, const Value& rhs);

		Value& operator*=(const Value& rhs);
		friend Value operator*(Value lhs, const Value& rhs);

		Value& operator/=(const Value& rhs);
		friend Value operator/(Value lhs, const Value& rhs);

		Value& operator--();
		Value& operator++();

		operator bool() const;
};


