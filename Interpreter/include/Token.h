#pragma once

#include "stdafx.h"

struct Token {
	enum Type {
		NOOP,				//
		NUMBER,				// 23
		IDENTIFIER,			// foo
		STRING,				// "foo"
		EOL,				// ;
		EOFILE,				// \0
		PLUS,				// +		
		MINUS,				// -
		STAR,				// *
		DIVIDE,				// /
		MODULO,				// %
		MODULO_ASSIGN,		// %=
		BITWISE_AND,		// &
		BITWISE_AND_ASSIGN,	// &=
		AND,				// &&
		AND_ASSIGN,			// &&=
		BITWISE_OR,			// |
		BITWISE_OR_ASSIGN,  // |=
		OR,					// ||
		OR_ASSIGN,			// ||=
		BITWISE_XOR,		// ^
		BITWISE_XOR_ASSIGN, // ^=
		INCREMENT,			// ++
		DECREMENT,			// --
		ADD_ASSIGN,			// +=
		SUBTRACT_ASSIGN,	// -=
		MULTIPLY_ASSIGN,	// *=
		DIVIDE_ASSIGN,		// /=
		ASSIGN,				// =
		EQ,					// ==
		STRICT_EQ,			// ===
		BITWISE_NOT,		// ~
		BITWISE_NOT_ASSIGN,	// ~=
		NOT,				// !
		INEQ,				// !=
		STRICT_INEQ,		// !==
		LT,					// <
		GT,					// >
		LTE,				// <=
		GTE,				// >=
		SHIFT_LEFT,			// <<
		SHIFT_RIGHT,		// >>
		SHIFT_LEFT_ASSIGN,	// <<=
		SHIFT_RIGHT_ASSIGN,	// >>=
		OPEN_BRACKET,		// (
		CLOSE_BRACKET,		// )
		OPEN_BRACE,			// {
		CLOSE_BRACE,		// }
		COMMA,				// ,
		DOT,				// .
		TRUE,				// true
		FALSE,				// false
		FUNCTION,			// function
		RETURN,				// return
		BREAK,				// break
		CONTINUE,			// continue
		IF,					// if
		ELSE,				// else
		WHILE,				// while
		FOR,				// for
		VAR,				// var
		THROW,				// throw
		TRY,				// try
		CATCH,				// catch
		TYPEOF,				// typeof
		UNDEFINED,			// undefined
	};

	static const map<string, Token::Type> KeyWords;

	Type type;
	double value;
	string name;
	Token() {};
	Token(Type t) : type(t), value(0) {}
	Token(Type t, double v) : type(t), value(v) {}
	Token(Type t, string n) : type(t), name(n) {}
};

