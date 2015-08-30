#pragma once

#include "stdafx.h"

class Value;
class Variable;

class Scope : public enable_shared_from_this<Scope> {

	public:
		typedef shared_ptr<Scope> Sptr;

		static int count;

		int depth = 0;
		Scope() : root(true) {
			count++;
		};
		Scope(Sptr p) : root(false), parent(move(p)){
			count++;
			depth = parent->depth + 1;
		};
		~Scope() {
			count--;
			if (!count) {
				debug("all scopes destroyed");
			}

		}
		void set(const Variable& v, Value value);
		Value get(const Variable& v);
		void define(const Variable& v, Value value);
		bool defined(const Variable& v);
		Sptr makeChild();
		void dispose();
		void print();

private:
	map<unsigned int, Value> vars;
	Sptr parent;
	bool root;
	bool disposed = false;
	vector<weak_ptr<Scope>> children;


};