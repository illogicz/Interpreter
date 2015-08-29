#include "stdafx.h"

#include "Value.h"
#include "Scope.h"
#include "Utils.h"
#include "Function.h"
#include "Statement.h"
#include "Jump.h"

Function::Function(vector<Variable> argument_names, Statement* statement) :
	argument_names(argument_names),
	statement(statement) {};

Function::~Function()
{
	if (statement != nullptr)
		delete statement;
}

Jump Function::execute(Scope::Sptr scope, vector<Value>& arguments) {
	auto childScope = make_shared<Scope>(scope);
	if (argument_names.size() != arguments.size()) {
		my_error("incorrect number of arguments");
	}
	for (size_t i = 0; i < arguments.size(); i++) {
		childScope->define(argument_names[i], arguments[i]);
	}
	return statement->execute(childScope);
}
