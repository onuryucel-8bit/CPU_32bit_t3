#pragma once

#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <iostream>

#include "Tokens.h"
#include "Lexer.h"

namespace algo
{
	extern std::unordered_map<asmc::TokenType, int> predence;
	
	class ShuntingYard
	{
	public:
		ShuntingYard();
		~ShuntingYard();

		void add(asmc::Token inp);
		std::vector<asmc::Token> calc();

	private:
		std::vector<asmc::Token> input;
	};	
}