#include "stdafx.h"

#include "Program.h"
#include "Statement.h"
#include "Environment.h"

Program::Program(Statement* js) : js(js)
{
	Environment* env = new Environment();
	scope = Scope::Sptr(env);
	env->init();
}

Program::~Program()
{

	scope->dispose();

	delete js;
}

Jump Program::execute() {
	Jump j = js->execute(scope);
	if (j.type != Jump::NONE) {
		cout << "uncaught statement termination";
	}
	return j;
}