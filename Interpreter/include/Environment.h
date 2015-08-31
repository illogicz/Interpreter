#pragma once

#include "Function.h"
#include "Jump.h"

static Jump ntv_print(IFunction::Args& args);

class Environment : public Scope {

	public:
		Environment() {};
		void init();

	private:
		void add(string var, NativeFunction::FuncRef ref);
};
