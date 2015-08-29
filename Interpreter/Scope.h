#pragma once

#include "stdafx.h"
#include "Utils.h"

class Value;
class Variable;

class Scope {

	public:
		typedef shared_ptr<Scope> Sptr;

		int depth = 0;
		Scope() : root(true) {
			cout << "root scope created" << endl;
		};
		Scope(Sptr p) : root(false) {
			parent = Scope::Sptr(p);
			cout << "child scope created, depth = " << p->depth + 1  << endl;
			depth = p->depth + 1;
		};
		~Scope()
		{
			debug("scope dtor " + to_string(depth));
		}
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