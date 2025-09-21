#pragma once
namespace asmc
{
	enum TokenType
	{

		ENDOFLINE = -1,

		LET,
		IF,
		THEN,
		END,
		WHILE,
		IDENT,
		STRING,
		NUMBER,
		
		PLUS,			// +
		MINUS,			// -
		ASTERISK,		// *
		SLASH,			// /
		EQUAL, 			// =
		SEMICOLON, 		// ;
		
		
		INCLUDE,
		DEFINE
		
		
	};
}