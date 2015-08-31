#pragma once

#include "stdafx.h"

#include "Token.h"

class TokenStream {
	public:
		TokenStream(istream& stream) :stream(stream){};
		void putback(Token token);
		void ignore();
		Token get();

	private:

		vector<Token> buffer;
		istream& stream;

		inline bool is_white(char ch);
		inline Token get_additive(char ch);
		inline Token get_number(char ch);
		inline Token get_string();
		inline Token get_if(char m, Token without, Token with);
		inline Token get_if(char m1, char m2, Token nomatch, Token matchfirst, Token matchboth);
		inline Token get_mat(char m1, char m2, Token match00, Token match10, Token match01, Token match11);

};