#pragma once

#include <cstdint>
#include "Tokens.h"


namespace asmc
{
	enum class Location
	{
		None,
		Constant,
		Register,
		Stack
	};



	struct ExprVal
	{
		asmc::Token m_token;
		asmc::Location m_location = Location::None;
		char m_registerIndex = -1;
		uint32_t m_value = 0;

		ExprVal()
		{

		}

		ExprVal(asmc::Token& token)
		{
			m_token = token;
		}
	};

	//------------------------------------------------------------------------//
	//------------------------------------------------------------------------//
	//------------------------------------------------------------------------//
	
	enum class NodeType
	{
		Constant,
		Variable,
		BinOp
	};

	struct Node
	{

		asmc::NodeType m_type;

		asmc::Node* m_left;
		asmc::Node* m_right;

	};	

	struct BinOpNode : Node
	{
		asmc::Token m_token;

		BinOpNode(asmc::Token token, asmc::Node* left, asmc::Node* right)
		{
			m_token = token;
			m_left = left;
			m_right = right;
		}
	};

	struct VariableNode : Node
	{
		//m_symbolTable[token].m_ramIndex => ramAdr
		asmc::Token m_token;
		char m_registerIndex = -1;
		asmc::Location m_location = asmc::Location::None;

		VariableNode(asmc::Token token, asmc::Node* left, asmc::Node* right)
		{
			m_token = token;
			m_left = left;
			m_right = right;
		}
	};
	
	struct ConstantNode : Node
	{
		uint32_t m_value;

		ConstantNode(uint32_t value, asmc::Node* left, asmc::Node* right)
		{
			m_value = value;
			m_left = left;
			m_right = right;
		}
	};
}