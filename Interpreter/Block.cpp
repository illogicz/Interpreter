#include "stdafx.h"

#include "Value.h"
#include "Scope.h"

#include "Block.h"

Block::Block() {};

Value Block::execute(Scope& scope)
{
	Scope blockScope = Scope(scope);
	for (size_t i = 0; i < statement.size(); i++) {
		Value return_value = statement[i].execute(blockScope);
		if (return_value.type != Value::NONE) {
			return return_value;
		}
	}
	return Value(Value::NONE);
}