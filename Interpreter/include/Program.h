#pragma once

#include "Statement.h"
#include "Environment.h"
#include "Jump.h"

class Program {
public:
	Program(Statement* js);
	~Program();
	Statement* js;
	Scope::Sptr scope;
	Jump execute();
};