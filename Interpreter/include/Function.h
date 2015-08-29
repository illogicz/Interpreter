#pragma once

#include "Scope.h"

class Variable;
class Statement;
class Value;
class Scope;
class Jump;

class Function {
	private:
		vector<Variable> argument_names;
		const Statement& statement;

	public:
		Function(vector<Variable> argument_names, const Statement& statement);
		~Function();
		Jump execute(Scope::Sptr scope, const vector<Value>& arguments);
		typedef shared_ptr<Function> Sptr;
};

