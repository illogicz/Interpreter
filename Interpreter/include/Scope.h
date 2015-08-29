#pragma once

#include "stdafx.h"
#include "Utils.h"

class Value;
class Variable;

class Scope {

	public:
		typedef shared_ptr<Scope> Sptr;

		int depth = 0;
		Scope() : root(true) {};
		Scope(Sptr p) : root(false) {
			parent = Scope::Sptr(p);
			depth = p->depth + 1;
		};
		void set(const Variable& v, Value value);
		Value get(const Variable& v);
		void define(const Variable& v, Value value);
		bool defined(const Variable& v);
		void dispose();
		void print();

private:
	map<unsigned int, Value> vars;
	Sptr parent;
	bool root;

};