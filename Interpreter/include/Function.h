#pragma once

#include "Scope.h"

class Variable;
class Statement;
class Value;
class Jump;

class IFunction {
	public:
		typedef const vector<Value> Args;
		virtual Jump execute(Scope::Sptr scope, Args& arguments) = 0;
		typedef shared_ptr<IFunction> Sptr;
};

class Function : public IFunction {
	private:
		vector<Variable> argument_names;
		const Statement& statement;

	public:
		Function(vector<Variable> argument_names, const Statement& statement);
		~Function();
		Jump execute(Scope::Sptr scope, Args& arguments);
};

class NativeFunction : public IFunction {
	public:
		typedef Jump(*FuncRef)(Args& arguments);
		FuncRef func_ref;
		NativeFunction(FuncRef func_ref);
		Jump execute(Scope::Sptr scope, Args& arguments);
};