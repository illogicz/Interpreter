#pragma once

#include "Value.h"

class Jump {
	public:
		enum Type {
			RETURN,
			BREAK,
			CONTINUE,
			ERROR,
			NONE
		};
		Type type;
		Value value;
		Jump() : type(NONE) {};
		Jump(Type type) : type(type), value(Value(Value::UNDEFINED)) {};
		Jump(Type type, Value value) : type(type), value(value) {};
};
