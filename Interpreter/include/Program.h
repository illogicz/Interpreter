#pragma once

#include "Statement.h"
#include "Scope.h"
#include "Jump.h"

class Program : public CompoundStatement {
public:
	Program();
	~Program();
	Scope::Sptr scope;
	Jump execute();
};