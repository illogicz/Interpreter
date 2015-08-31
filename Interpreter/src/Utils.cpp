#include "stdafx.h"

void error(const string& message)
{
	throw exception(message.c_str());
}

void debug(string s) {
	cout << s << endl;
}
