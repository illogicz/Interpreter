#include "stdafx.h"
#include "Utils.h"

void my_error(const string& message)
{
	throw exception(message.c_str());
}

void debug(string s) {
	cout << s << endl;
}
