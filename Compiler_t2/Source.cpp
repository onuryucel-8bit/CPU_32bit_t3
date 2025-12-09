

#include "Lexer.h"

asmc::Lexer lexer("source.bc");
asmc::Token token = lexer.getToken();

int main()
{
	
	std::cout << "[text],[type]\n----------\n";
	while (token.m_type != asmc::TokenType::ENDOFFILE)
	{
		
		if (token.m_type != asmc::TokenType::NEWLINE)
		{
			std::cout <<"["<< token.m_text << "]  ,  [" << magic_enum::enum_name(token.m_type) << "]\n";
		}

		token = lexer.getToken();
	}

	
}