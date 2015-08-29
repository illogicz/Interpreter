#include "stdafx.h"
#include "Utils.h"
#include "TokenStream.h"

void TokenStream::putback(Token token)
{
	buffer.push_back(token);
}

void TokenStream::ignore()
{
	while (true) {
		try {
			if (get().type == Token::EOL)return;
		}
		catch (exception e) {
		}
	}
}

Token TokenStream::get()
{
	if (buffer.size()) {
		Token t = buffer.back();
		buffer.pop_back();
		return t;
	}

	char ch = stream.get();
	while (is_white(ch))ch = stream.get();

	if (stream.eof()) {
		
		return Token(Token::EOFILE);
	}

	switch (ch) {
	case ';': return Token(Token::EOL);
	case '(': return Token(Token::OPEN_BRACKET);
	case ')': return Token(Token::CLOSE_BRACKET);
	case '{': return Token(Token::OPEN_BRACE);
	case '}': return Token(Token::CLOSE_BRACE);
	case ',': return Token(Token::COMMA);
	case '%': return get_if('=', Token::MODULO, Token::MODULO_ASSIGN);
	case '^': return get_if('=', Token::BITWISE_XOR, Token::BITWISE_XOR_ASSIGN);
	case '~': return get_if('=', Token::BITWISE_NOT, Token::BITWISE_NOT_ASSIGN);
	case '*': return get_if('=', Token::STAR, Token::MULTIPLY_ASSIGN);
	case '=': return get_if('=', '=', Token::ASSIGN, Token::EQ, Token::STRICT_EQ);
	case '!': return get_if('=', '=', Token::NOT, Token::INEQ, Token::STRICT_INEQ);
	case '&': return get_mat('&', '=', Token::BITWISE_AND, Token::AND, Token::BITWISE_AND_ASSIGN, Token::AND_ASSIGN);
	case '|': return get_mat('|', '=', Token::BITWISE_OR, Token::OR, Token::BITWISE_OR_ASSIGN, Token::OR_ASSIGN);
	case '<': return get_mat('<', '=', Token::LT, Token::SHIFT_LEFT, Token::LTE, Token::SHIFT_LEFT_ASSIGN);
	case '>': return get_mat('>', '=', Token::GT, Token::SHIFT_RIGHT, Token::GTE, Token::SHIFT_RIGHT_ASSIGN);
	case '/': {
		ch = stream.get();
		switch(ch){
			case '/':
				while (stream && stream.get() != '\n');
				return get();
			case '*':
				while (stream && !(stream.get() == '*' && stream.get() == '/'));
				return get();
			default:
				stream.putback(ch);
				return get_if('=', Token::SLASH, Token::DIVIDE_ASSIGN);
		}
	}
	case '-':
	case '+': return get_additive(ch);
	case '"': return get_string();
	case '.': {
		char ch2 = stream.get();
		if (!isdigit(ch2))
			return Token(Token::DOT);
		stream.putback(ch2);
	}
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9': return get_number(ch);
	default: {
		if (isalpha(ch) || ch == '_') {
			string name = "";
			name += ch;
			while (stream.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) name += ch;
			stream.putback(ch);

			Token::Type t = Token::NAME;

			
				 if (name == KEYW_ELSE)		t = Token::ELSE;
			else if (name == KEYW_IF)		t = Token::IF;
			else if (name == KEYW_FOR)		t = Token::FOR;
			else if (name == KEYW_WHILE)	t = Token::WHILE;
			else if (name == KEYW_FUNCTION) t = Token::FUNCTION;
			else if (name == KEYW_VAR)		t = Token::VAR;
			else if (name == KEYW_RETURN)	t = Token::RETURN;
			else if (name == KEYW_TRUE)		t = Token::TRUE;
			else if (name == KEYW_FALSE)	t = Token::FALSE;

			if (t == Token::NAME)
				return Token(t, name);
			else
				return Token(t);

		}
		throw exception("invalid token: ");
	}
	}
	return Token(Token::EOFILE);
}

inline bool TokenStream::is_white(char ch)
{
	switch (ch) {
	case ' ':
	case '\n':
	case '\r':
	case '\t': return true;
	default: return false;
	}
}

inline Token TokenStream::get_additive(char ch)
{
	char ch2 = stream.get();
	if (ch == ch2) {
		return Token(ch == '-' ? Token::DECREMENT : Token::INCREMENT);
	}
	else if (ch2 == '=') {
		return Token(ch == '-' ? Token::SUBTRACT_ASSIGN : Token::ADD_ASSIGN);
	}
	else {
		stream.putback(ch2);
		return Token(ch == '-' ? Token::MINUS : Token::PLUS);
	}
}

inline Token TokenStream::get_number(char ch)
{
	stream.putback(ch);
	double d;
	stream >> d;
	if (!stream) {
		stream.clear();
		throw exception("invalid number");
	}
	return Token(Token::NUMBER, d);
}

inline Token TokenStream::get_string()
{
	string str = "";
	char ch = stream.get();
	while (ch != '"') {
		if (ch == '\\') {
			char ch2 = stream.get();
				switch (ch2) {
				case 'n': ch = '\n';
				case 'r': ch = '\r';
				case '\\':
				case '"': ch = ch2;
				default: my_error("Invalid escape sequence");
			}
		}
		str.append(1, ch);
		ch = stream.get();
	}
	return Token(Token::STRING, str);
};

inline Token TokenStream::get_if(char m, Token without, Token with)
{
	char ch = stream.get();
	if (ch == m) {
		return Token(with);
	}
	stream.putback(ch);
	return Token(without);
};

inline Token TokenStream::get_if(char m1, char m2, Token nomatch, Token matchfirst, Token matchboth)
{
	char ch = stream.get();
	if (ch == m1) {
		return get_if(m2, matchfirst, matchboth);
	}
	stream.putback(ch);
	return Token(nomatch);
};

inline Token TokenStream::get_mat(char m1, char m2, Token match00, Token match10, Token match01, Token match11)
{
	char ch2 = stream.get();
	if (ch2 == m1) {
		return get_if(m2, match10, match11);
	}
	else if (ch2 == m2) {
		return Token(match01);
	}
	else {
		stream.putback(ch2);
		return Token(match11);
	}
}
