#pragma once

#include "Lexer.h"
namespace asmc
{

	class Parser
	{
	public:
		Parser(asmc::Lexer& lexer);
		~Parser();

		void run();

	private:
		asmc::Lexer& m_lexer;
	};

}
