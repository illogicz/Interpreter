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
		Jump(Type type);
		Jump(Value value);
};
