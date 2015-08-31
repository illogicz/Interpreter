#include "stdafx.h"

#include "Environment.h"
#include "Expression.h"

void Environment::init() {
	//Scope();
	add("print", &ntv_print);
}



Jump ntv_print(IFunction::Args& args)
{
	for (size_t i = 0; i < args.size(); i++){
		if (i) debug(", ");
		debug(string(args[0]));
	}
	debug("\n");
	return Jump();
}


void Environment::add(string var, NativeFunction::FuncRef ref)
{
	auto fn = IFunction::Sptr(new NativeFunction(ref));
	this->define(Variable(var), Value(fn, shared_from_this()));
};

