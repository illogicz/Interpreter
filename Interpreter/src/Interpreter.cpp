#include "stdafx.h"

#include "Parser.h"
#include "Program.h"

int main()
{
	{
		// open file stream
		ifstream file("code.js");

		// make token stream
		TokenStream ts(file);

		// token parser
		Parser prs;

		cout << "compiling file" << endl;

		// Parse file
		Program& program = *prs.parse(&ts);

		file.close();


		cout << "end of file reached" << endl;
		cout << "executing.." << endl;

		try {
			program.execute();
		}
		catch (exception e) {
			cout << "Runtime error: " << e.what() << endl;
		}
		cout << "done" << endl;
		cout << "----------" << endl;
		program.scope->print();
		cout << "----------" << endl;

		delete &program;

	}

	while (cin) {
		char ch;
		cin >> ch;
		cout << ch;
	}
	return 0;
}