#pragma once

#include "Statement.h"

class Value;
class Scope;
class Statement;

class Block {
	private:
		vector<Statement> statement;
		
	public:
		Block();
		Value execute(Scope& scope);

};