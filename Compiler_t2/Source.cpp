/*

	LET A = 5
	LET B = 3

	//A = 3
	A = B

	//A = 4
	A = A + 1

	IF A > 0 THEN
		
		//4
		LET C = A
		//7 = 4 + 3
		C = C + B

	ENDIF

	WHILE C > 0 

		C = C - 1

	WEND

	//SUM() ?
	//GOTO SUM() ?
	GO SUM()

	RETURN

	FUNC SUM()		
		A = A + 1
		RETURN

		


*/

/*
--------------
 2+5
 13+24
--------------
 LET A = 5
--------------
 A = A + 1
 A = A - 1
--------------
*/

#include "Lexer.h"

int main()
{
	asmc::Lexer lexer("source.bc");
	
	asmc::Token token = lexer.getToken();	


	
	while (token.m_type != asmc::TokenType::ENDOFFILE)
	{
		std::cout << token.m_text << "|" << magic_enum::enum_name(token.m_type) <<"\n";
		

		if (token.m_type == asmc::TokenType::NUMBER)
		{
			std::cout << "LOAD r0," << token.m_text << "\n";
		}
		token = lexer.getToken();
	}

	
}