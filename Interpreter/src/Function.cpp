#include "stdafx.h"

#include "Value.h"
#include "Scope.h"
#include "Function.h"
#include "Statement.h"
#include "Jump.h"

Function::Function(vector<Variable> argument_names, const Statement& statement) :
	argument_names(argument_names),
	statement(statement) {};

Function::~Function()
{
	if (&statement != nullptr)
		delete &statement;
}

Jump Function::execute(Scope::Sptr scope, const vector<Value>& arguments) {
	auto childScope = scope->makeChild();
	if (argument_names.size() != arguments.size()) {
		error("incorrect number of arguments");
	}
	for (size_t i = 0; i < arguments.size(); i++) {
		childScope->define(argument_names[i], arguments[i]);
	}
	Jump j = statement.execute(childScope);
	switch (j.type) {
		case Jump::BREAK: error("uncaught break statement");
		case Jump::CONTINUE: error("uncaught continue statement");
		default: return j;
	}
}


NativeFunction::NativeFunction(FuncRef func_ref) 
	: func_ref(func_ref) {};


Jump NativeFunction::execute(Scope::Sptr scope, Args& arguments) {
	return func_ref(arguments);
}
