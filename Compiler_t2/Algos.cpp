#include "Algos.h"

namespace algo
{
	std::unordered_map<asmc::TokenType, int> predence =
	{
		{asmc::TokenType::PLUS,  1 },
		{asmc::TokenType::MINUS, 1 },
		{asmc::TokenType::ASTERISK,  2 },
		{asmc::TokenType::SLASH,   2 }
	};

	ShuntingYard::ShuntingYard()
	{
	}

	ShuntingYard::~ShuntingYard()
	{
	}

	void ShuntingYard::add(asmc::Token inp)
	{
		input.push_back(inp);
	}

	std::vector<asmc::Token> ShuntingYard::calc()
	{
		std::stack<asmc::Token> stack;
		std::vector<asmc::Token> output;
		

		for (int i = 0; i < input.size(); i++)
		{
			asmc::Token token = input[i];

			if (token.m_type == asmc::TokenType::NUMBER || token.m_type == asmc::TokenType::ID)
			{
				output.push_back(input[i]);
			}
			else if (token.m_type == asmc::TokenType::LPAREN)
			{
				stack.push(token);
			}
			else if (token.m_type == asmc::TokenType::RPAREN)
			{
				//pop all 
				asmc::Token topChar = stack.top();
				while (topChar.m_type != asmc::TokenType::LPAREN && !stack.empty())
				{
					output.push_back(topChar);

					stack.pop();

					topChar = stack.top();
				}

				//pop '('
				stack.pop();
			}
			else
			{
				if (stack.empty())
				{
					stack.push(token);
				}
				else if (predence[token.m_type] <= predence[stack.top().m_type])
				{
					output.push_back(stack.top());
					stack.pop();

					stack.push(token);
				}
				else if (predence[token.m_type] > predence[stack.top().m_type])
				{
					stack.push(token);
				}
			}
		}

		while (!stack.empty())
		{
			output.push_back(stack.top());
			stack.pop();
		}

		for (size_t i = 0; i < output.size(); i++)
		{
			std::cout << output[i].m_text << "\n";
		}

		return output;
	}
}