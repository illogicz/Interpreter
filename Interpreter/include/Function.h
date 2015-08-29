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
		Statement* statement;

	public:
		Function() {};
		Function(vector<Variable> argument_names, Statement* statement);
		~Function();
		Jump execute(Scope::Sptr scope, vector<Value>& arguments);
		typedef shared_ptr<Function> Sptr;
};

