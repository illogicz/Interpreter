#pragma once

#include "Function.h"
#include "Scope.h"

class Value {
	public:
		enum Type {
			UNDEFINED,
			BOOLEAN,
			NUMBER,
			STRING,
			FUNCTION
			// ARRAY,
			// OBJECT,
		};


		Value();
		Value(Type t);
		Value(double v);
		Value(string v);
		Value(bool v);
		Value(IFunction::Sptr f, Scope::Sptr s);

		Value& operator +=  (const Value& rhs);
		Value& operator -=  (const Value& rhs);
		Value& operator *=  (const Value& rhs);
		Value& operator /=  (const Value& rhs);
		Value& operator %=  (const Value& rhs);
		Value& operator <<= (const Value& rhs);
		Value& operator >>= (const Value& rhs);
		Value& operator --  ();
		Value& operator ++  ();

		Value operator + () const;
		Value operator - () const;
		Value operator ! () const;

		Value typeof() const;

		friend Value operator +  (Value lhs, const Value& rhs);
		friend Value operator -  (Value lhs, const Value& rhs);
		friend Value operator *  (Value lhs, const Value& rhs);
		friend Value operator /  (Value lhs, const Value& rhs);
		friend Value operator %  (Value lhs, const Value& rhs);
		friend Value operator << (Value lhs, const Value& rhs);
		friend Value operator >> (Value lhs, const Value& rhs);
		friend Value operator || (Value lhs, const Value& rhs);
		friend Value operator && (Value lhs, const Value& rhs);

		bool equality    (const Value& value) const;
		bool inequality  (const Value& value) const;
		bool operator == (const Value& value) const;
		bool operator != (const Value& value) const;
		bool operator <  (const Value& value) const;
		bool operator <= (const Value& value) const;
		bool operator >  (const Value& value) const;
		bool operator >= (const Value& value) const;

		inline operator string() const;
		inline operator double() const;
		inline operator int()    const;
		inline operator bool()   const;

		Value operator()(const vector<Value>& args) const;


	private:
		Type type;
		double num;
		string str;
		bool bln;
		IFunction::Sptr func;
		Scope::Sptr closure;

};


