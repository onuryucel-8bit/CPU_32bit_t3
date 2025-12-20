#include "Parser.h"
namespace asmc
{


	Parser::Parser(asmc::Lexer& lexer)
		:m_lexer(lexer)
	{
		m_tokenList = lexer.getTokenList();

		
		f_error = false;
		
		m_tokenIndex = 0;
		m_peekToken = m_tokenList[0];
		moveCurrentToken();

		
	}

	Parser::~Parser()
	{
	}




	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//

#pragma region PRINTS
	void Parser::printError(std::string message)
	{
		std::cout << rang::fg::red 
			<< "ERROR:: "
			<< message
			<< " :: line number["<< m_currentToken.m_lineNumber << "] "
			<< " currentToken ["+ m_currentToken.m_text +"]" 
			<< " type ["+ std::string(magic_enum::enum_name(m_currentToken.m_type)) + "] "			
			<< rang::style::reset << "\n";

		f_error = true;
	}

	void Parser::printErrorExt(std::string message, asmc::Token token)
	{
		std::cout << rang::fg::red
			<< "ERROR:: "
			<< message
			<< " :: line number[" << m_currentToken.m_lineNumber << "] "
			<< " token [" + token.m_text + "]"
			<< " type [" + std::string(magic_enum::enum_name(token.m_type)) + "] "
			<< rang::style::reset << "\n";

		f_error = true;
	}

	void Parser::emit(std::string str)
	{
		if (!f_error)
		{
			std::cout << str << "\n";
		}	
	}

	void Parser::printWarning(std::string message)
	{
		std::cout << rang::fg::yellow << "WARNING:: " << message << rang::style::reset << "\n";
	}
#pragma endregion

	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------//


	
	void Parser::run()
	{
		std::cout << "Lexer output...\n"
				  << "===========================\n";

		for (asmc::Token t : m_tokenList)
		{
			std::cout << t <<"\n";

			if (t.m_type == asmc::TokenType::ENDOFFILE)
			{
				break;
			}
		}

		std::cout << "Parser running... \n"
				  << "===========================\n";

		while (m_currentToken.m_type != asmc::TokenType::ENDOFFILE)
		{
			//std::cout << m_currentToken.m_text << "\n";
			program();
		}

		std::cout << "===========================\n"
				  << "Printing VAR-SECTION...\n";

		emit(".origin 1000");
		for (variableVec& vec : m_varlist)
		{
			std::cout << vec.m_ramIndex << " = ";
			emit(".db " + std::to_string(vec.m_value));
		}


		std::cout << "===========================\n"
				  << "Printing symbol table...\n";
		for (const auto& [key, value] : m_symbolTable)
		{
			std::cout << key.m_text << "|" << value.m_value << "\n";

			if (value.m_status == asmc::symbolStatus::NOT_USED)
			{
				printWarning("variable[" + key.m_text + "] is not used");
			}
		}



	}


	/*
			
	*/
	void Parser::program()
	{
		while (m_currentToken.m_type ==  asmc::TokenType::LET ||
			   m_currentToken.m_type == asmc::TokenType::ID)

		{			

			switch (m_currentToken.m_type)
			{
			case asmc::TokenType::LET:
				let_stmt();				
				break;

			case asmc::TokenType::ID:
				assing_stmt();
				break;
			}
			
		}
		
	}

	//"LET" <id> "=" <expression> ";"
	void Parser::let_stmt()
	{
		match(asmc::TokenType::LET);

		asmc::ExprVal idName = expression();		
		
		if (m_symbolTable.contains(idName.m_token))
		{
			printErrorExt("variable defined twice", idName.m_token);
		}

		//insert the variable to table with default type
		m_symbolTable[idName.m_token];
		
		if (m_currentToken.m_type == asmc::TokenType::ASSIGN)
		{
			match(asmc::TokenType::ASSIGN);

			asmc::ExprVal value = expression();


			uint32_t entryVal = std::stoi(value.m_token.m_text);
			uint32_t ramIndex = m_memManager.allocVariable();

			//insert variable to the table
			m_symbolTable[idName.m_token].m_value = entryVal;			
			m_symbolTable[idName.m_token].m_ramIndex = ramIndex;			

			asmc::variableVec varvec = {.m_value = entryVal ,.m_ramIndex = ramIndex };
			m_varlist.push_back(varvec);										
		
		};
			
		match(asmc::TokenType::SEMICOLON);
	}

	//<id> "=" <expression>*
	void Parser::assing_stmt()
	{
		asmc::ExprVal idRes = id();	

		if (!m_symbolTable.contains(idRes.m_token))
		{
			printErrorExt("id is not defined", idRes.m_token);
		}
		else
		{
			m_symbolTable[idRes.m_token].m_status = asmc::symbolStatus::USED;

			idRes.m_value = m_symbolTable[idRes.m_token].m_value;
		}

		match(asmc::TokenType::ASSIGN);

		asmc::ExprVal result = add_expr();


		if (result.m_location == asmc::Location::None)
		{
			emit("STR  @" + getAdr_str(idRes) + ",0x" + std::to_string(result.m_value));
		}
		else
		{
			emit("STR  @" + getAdr_str(idRes) + ",r" + std::to_string((int)result.m_registerIndex));
		}

		m_symbolTable[idRes.m_token].m_value = result.m_value;

		//release register
		m_memManager.releaseRegister(result.m_registerIndex);		
	}

	//<add_expr> ::= <expression> ((+ | -) <expression>)* ";"
	asmc::ExprVal Parser::add_expr()
	{
		asmc::ExprVal exprLeft = expression();				
		

		#pragma region ErrorCheck
		//check id is defined

		checkTable(exprLeft);		

		if (exprLeft.m_token.m_type == asmc::TokenType::ID)
		{
			m_symbolTable[exprLeft.m_token].m_status = asmc::symbolStatus::USED;
			exprLeft.m_value = m_symbolTable[exprLeft.m_token].m_value;
		}
		else
		{
			exprLeft.m_value = std::stoi(exprLeft.m_token.m_text);
		}
		#pragma endregion

		asmc::ExprVal exprRight;

		while (m_currentToken.m_type == asmc::TokenType::PLUS || m_currentToken.m_type == asmc::TokenType::MINUS)
		{
			//get operator
			asmc::Token op = m_currentToken;
			
			moveCurrentToken();
			exprRight = expression();

			checkTable(exprRight);

			//get value, change status of id
			if (exprRight.m_token.m_type == asmc::TokenType::ID)
			{
				exprRight.m_value = m_symbolTable[exprRight.m_token].m_value;
				m_symbolTable[exprRight.m_token].m_status = asmc::symbolStatus::USED;
			}
					
			if (exprLeft.m_token.m_type == asmc::TokenType::ID)
			{
				exprLeft.m_rhsComputed = true;
			}

			switch (op.m_type)
			{
			case asmc::TokenType::PLUS:
				exprLeft.m_value += exprRight.m_value;				
				break;

			case asmc::TokenType::MINUS:
				exprLeft.m_value -= exprRight.m_value;
				break;
			}

		}//WHILE END

		match(asmc::TokenType::SEMICOLON);

		//load register
		loadToRegister(exprLeft);

		return exprLeft;
	}

	//<mult_expr> ::= (<id> | <number>) ((* | /) (<id> | <number>))*
	asmc::Token Parser::mult_expr()
	{
		asmc::Token idLeft;

		//switch (m_currentToken.m_type)
		//{
		//case asmc::TokenType::ID:
		//	idLeft = id();
		//	if (!m_symbolTable.contains(idLeft))
		//	{
		//		printErrorExt("undefined variable used ", idLeft);
		//	}
		//	else
		//	{
		//		m_symbolTable[idLeft].m_status = asmc::symbolStatus::USED;
		//	}

		//	//emit("VAR " + idLeft.m_text);
		//	break;

		//case asmc::TokenType::NUMBER:
		//	idLeft = number();
		//	//emit("NUMBER ," + idLeft.m_text);
		//	break;

		//default:
		//	printError("expected <ID> or <NUMBER> found,");
		//	break;
		//}		
		//
		////TODO fix idRight

		//asmc::Token idRight;

		//while (checkType(asmc::TokenType::ASTERISK) || checkType(asmc::TokenType::SLASH))
		//{

		//	asmc::Token op = m_currentToken;
		//	

		//	moveCurrentToken();

		//	if (checkType(asmc::TokenType::ID))
		//	{
		//		idRight = id();
		//		emit("OPERATOR," + op.m_text + "," + idRight.m_text);
		//	}
		//	else if (checkType(asmc::TokenType::NUMBER))
		//	{
		//		idRight = number();
		//		switch (op.m_type)
		//		{
		//		case asmc::TokenType::ASTERISK:
		//			emit("MUL r" + std::to_string(m_symbolTable[idLeft].m_registerIndex) + ", 0x" + rdx::decToHex(std::stoi(idRight.m_text)));
		//			break;
		//		case asmc::TokenType::DIV:
		//			emit("DIV r" + std::to_string(m_symbolTable[idLeft].m_registerIndex) + ", 0x" + rdx::decToHex(std::stoi(idRight.m_text)));
		//			break;
		//		}
		//	}
		//	else
		//	{
		//		printError("expected <ID> or <NUMBER> found,");
		//	}
		//}
		//
		return idLeft;
	}

	//<expression> ::= <id> | <number>
	asmc::ExprVal Parser::expression()
	{

		asmc::ExprVal retval;

		if (m_currentToken.m_type == asmc::TokenType::NUMBER)
		{
			retval = number();
		}
		else if(m_currentToken.m_type == asmc::TokenType::ID)
		{						
			retval = id();			
		}
		else
		{
			printError("id or number expected at");
		}

		
		return retval;
	}

	//<id>
	asmc::ExprVal Parser::id()
	{

		asmc::ExprVal retval;

		if (m_currentToken.m_type == asmc::TokenType::ID)
		{
			retval.m_token = m_currentToken;			
		}
		else
		{
			printError("expected <ID>, found");
		}

		moveCurrentToken();

		return retval;
	}

	//<number>
	asmc::ExprVal Parser::number()
	{

		asmc::ExprVal retval;
		if (m_currentToken.m_type == asmc::TokenType::NUMBER)
		{
			retval.m_token = m_currentToken;
			retval.m_value = std::stoi(m_currentToken.m_text);
		}
		else
		{
			printError("expected <NUMBER>, found");
		}

		moveCurrentToken();

		return retval;

	}


	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//

	void Parser::checkTable(asmc::ExprVal& expval)
	{
		//check id is defined
		if (expval.m_token.m_type == asmc::TokenType::ID && !m_symbolTable.contains(expval.m_token))
		{
			printErrorExt("id is not defined", expval.m_token);
		}
	}

	void Parser::match(asmc::TokenType type)
	{
	
		if (m_currentToken.m_type != type)
		{
			printErrorExt("; expected end of the line", m_currentToken);

			//printError
			//f_error = true
		}
		moveCurrentToken();
	}

	void Parser::moveCurrentToken()
	{
		
		m_currentToken = m_peekToken;

		m_tokenIndex++;
		if (m_tokenIndex >= m_tokenList.size())
		{
			m_tokenList = m_lexer.getTokenList();
			m_tokenIndex = 0;
		}

		m_peekToken = m_tokenList[m_tokenIndex];
	}

	void Parser::loadToRegister(asmc::ExprVal& expval)
	{
		if (expval.m_location == asmc::Location::Register)
		{
			return;
		}

		if (!m_memManager.allocRegister())
		{
			//print error
		}
		expval.m_registerIndex = m_memManager.m_activeRegister;
		expval.m_location = Location::Register;

		if (expval.m_token.m_type == asmc::TokenType::ID && expval.m_rhsComputed == false)
		{
			emit("LOAD r" + std::to_string(expval.m_registerIndex) + ",@" + std::to_string(m_symbolTable[expval.m_token].m_ramIndex));
		}
		//asmc::TokenType::NUMBER
		else
		{
			emit("LOAD r" + std::to_string(expval.m_registerIndex) + ",0x" + std::to_string(expval.m_value));
		}

	}

	//TODO =>FileWriter.h
	void Parser::instruction(asmc::TokenType operation, int registerIndex, int opcodeType, int value)
	{
		char type = 'E';

		switch (opcodeType)
		{
		case 0:
			type = '@';
			break;
		case 1:
			type = 'r';
			break;
		case 2:
			type = 'h';
			break;
		}

		std::string opcode;

		switch (operation)
		{	
		case asmc::TokenType::PLUS:
			opcode = "ADD";			
			break;

		case asmc::TokenType::MINUS:
			opcode = "SUB";			
			break;

		case asmc::TokenType::ASTERISK:
			opcode = "MUL";			
			break;
		}

		emit(opcode + " r" + std::to_string(registerIndex) + ", " + type + std::to_string(value));
			
	}

}