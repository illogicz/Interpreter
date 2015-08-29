#pragma once

#include "stdafx.h"

const string KEYW_VAR = "var";
const string KEYW_FUNCTION = "function";
const string KEYW_IF = "if";
const string KEYW_ELSE = "else";
const string KEYW_RETURN = "return";
const string KEYW_WHILE = "while";
const string KEYW_FOR = "for";
const string KEYW_TRUE = "true";
const string KEYW_FALSE = "false";


struct Token {
	enum Type {
		NOOP,				//
		NUMBER,				// 23
		NAME,				// foo
		STRING,				// "foo"
		EOL,				// ;
		EOFILE,				// \0
		PLUS,				// +		
		MINUS,				// -
		STAR,				// *
		SLASH,				// /
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
		IF,					// if
		ELSE,				// else
		WHILE,				// while
		FOR,				// for
		VAR,				// var
	};

	Type type;
	double value;
	string name;
	Token(Type t) : type(t), value(0) {}
	Token(Type t, double v) : type(t), value(v) {}
	Token(Type t, string n) : type(t), name(n) {}
};
