#pragma once

#include "Scope.h"

class Value;

class IEvalable {
public:
	virtual ~IEvalable() {}
	IEvalable() {}
	typedef unique_ptr<IEvalable> Uptr;
	virtual Value operator()(Scope::Sptr scope) = 0;
};

