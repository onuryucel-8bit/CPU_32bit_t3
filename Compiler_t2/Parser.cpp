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

		
		std::cout << ".db ";
		/*for (size_t i = 0; i < m_varlist.size(); ++i)
		{
			std::cout << std::hex << m_varlist[i].m_value;
			if (i + 1 < m_varlist.size())
			{
				std::cout << ",";
			}
		}*/
		std::cout << "\n";

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
			   m_currentToken.m_type == asmc::TokenType::ID ||
			   m_currentToken.m_type == asmc::TokenType::PRINT)

		{			

			switch (m_currentToken.m_type)
			{
			case asmc::TokenType::LET:
				let_stmt();				
				break;

			case asmc::TokenType::ID:
				assing_stmt();
				break;

			case asmc::TokenType::PRINT:
				print_stmt();
				break;
			}
			
		}
		
	}

	//"LET" <id> ([ <number> ])? "=" (<expression> | ({ <expression>* })? ) ";"
	void Parser::let_stmt()
	{
		match(asmc::TokenType::LET);

		asmc::ExprVal idName = primary();		
		
		if (m_symbolTable.contains(idName.m_token))
		{
			printErrorExt("variable defined twice", idName.m_token);
		}

		//insert the variable to table with default type
		m_symbolTable[idName.m_token];
		
		if (m_currentToken.m_type == asmc::TokenType::ASSIGN)
		{
			match(asmc::TokenType::ASSIGN);

			//LET a = "asdasfasfsdaf";
			if (m_currentToken.m_type == asmc::TokenType::STRING)
			{				
			
				uint32_t entryVal = (uint32_t)(m_currentToken.m_text[0]);
				uint32_t ramIndex = m_memManager.allocVariable();

				asmc::variableVec varvec = { .m_value = entryVal ,.m_ramIndex = ramIndex };
				m_varlist.push_back(varvec);

				//insert variable to the table
				m_symbolTable[idName.m_token].m_value = entryVal;
				m_symbolTable[idName.m_token].m_ramIndex = ramIndex;
				m_symbolTable[idName.m_token].m_type = asmc::variableType::String;

				//insert all chars to variable_sector
				for (size_t i = 1; i < m_currentToken.m_text.length(); i++)
				{
					uint8_t cha = m_currentToken.m_text[i];
					uint32_t ramIndex = m_memManager.allocVariable();

					asmc::variableVec varvec = { .m_value = (uint32_t)cha ,.m_ramIndex = ramIndex };
					m_varlist.push_back(varvec);
				}

				match(asmc::TokenType::STRING);
			}
			else
			{
				asmc::ExprVal value = primary();

				uint32_t entryVal = std::stoi(value.m_token.m_text);
				uint32_t ramIndex = m_memManager.allocVariable();

				//insert variable to the table
				m_symbolTable[idName.m_token].m_value = entryVal;
				m_symbolTable[idName.m_token].m_ramIndex = ramIndex;
				
				asmc::variableVec varvec = { .m_value = entryVal ,.m_ramIndex = ramIndex };
				m_varlist.push_back(varvec);
			}														
		}
		//"LET" <id> ([ <number> ])? "=" (<expression> | ({ <expression>* })? ) ";"
		else if (m_currentToken.m_type == asmc::TokenType::LBRACE)
		{
			match(asmc::TokenType::LBRACE);

			asmc::ExprVal arrSize = number();

			match(asmc::TokenType::RBRACE);
			match(asmc::TokenType::ASSIGN);
			match(asmc::TokenType::LCPAREN);

			std::vector<ExprVal> elements;
			elements.reserve(arrSize.m_value);

			while (m_currentToken.m_type == asmc::TokenType::NUMBER)
			{
				asmc::ExprVal elem = number();
				elements.push_back(elem);			
			}

			if (elements.size() != arrSize.m_value)
			{
				printError("arrSize is ");
			}

			for (asmc::ExprVal& elem : elements)
			{
				uint32_t entryVal = std::stoi(elem.m_token.m_text);
				uint32_t ramIndex = m_memManager.allocVariable();
				asmc::variableVec varvec = { entryVal ,ramIndex };
				m_varlist.push_back(varvec);
			}

			match(asmc::TokenType::RCPAREN);
		}
			
		
		match(asmc::TokenType::SEMICOLON);
	}

	//<print_stmt> ::= PRINT( <id> );
	void Parser::print_stmt()
	{
		match(asmc::TokenType::PRINT);
		match(asmc::TokenType::LPAREN);
		
		asmc::ExprVal exprId = id();

		checkTable(exprId);

		m_symbolTable[exprId.m_token].m_status = asmc::symbolStatus::USED;

		std::string reg;
		std::string sourceAdr;
		uint32_t arrPointer;

		switch (m_symbolTable[exprId.m_token].m_type)
		{
		case asmc::variableType::Int32:

				if (!m_memManager.allocRegister())
				{
					//print error
				}
				reg = std::to_string(m_memManager.m_activeRegister);
				sourceAdr = getAdr_asString(exprId);

				emit("LOAD r" + reg + ",@" + sourceAdr);
				emit("STR @" + std::to_string(asmc_TTY_adr) + ",r" + reg);

				

				//release register
				m_memManager.releaseRegister(m_memManager.m_activeRegister);
			break;

		case asmc::variableType::String:
			//load r0,@a
			//str @tty,r0
			//load r0,@a + 1

			//if (!m_memManager.allocRegister())
			//{
			//	//print error
			//}
			//reg = std::to_string(m_memManager.m_activeRegister);
			//arrPointer = m_symbolTable[exprId.m_token].m_ramIndex;

			//TODO
			/*while (??)
			{				
				emit("LOAD r" + reg + ",@" + std::to_string(arrPointer));
				emit("STR @" + std::to_string(asmc_TTY_adr) + ",r" + reg);

				arrPointer++;
			}*/

			break;
		}
			
		match(asmc::TokenType::RPAREN);
		match(asmc::TokenType::SEMICOLON);
			
	}

	//<id> "=" <add_expr>* ";"
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

		asmc::ExprVal result = expression();


		if (result.m_location == asmc::Location::None)
		{
			emit("STR  @" + getAdr_asString(idRes) + ",0x" + std::to_string(result.m_value));
		}
		else
		{
			emit("STR  @" + getAdr_asString(idRes) + ",r" + std::to_string((int)result.m_registerIndex));
		}

		m_symbolTable[idRes.m_token].m_value = result.m_value;

		//release register
		m_memManager.releaseRegister(result.m_registerIndex);	

		match(asmc::TokenType::SEMICOLON);
	}

	//<expression> ::= <add_expr> (("<" | "<=" | ">" | ">=" | "==" | "!=") <add_expr>)?
	asmc::ExprVal Parser::expression()
	{
		asmc::ExprVal exprLeft = add_expr();

//
//#pragma region ErrorCheck
//		//check id is defined
//		if (exprLeft.m_recRet == false)
//		{
//			checkTable(exprLeft);
//
//			if (exprLeft.m_token.m_type == asmc::TokenType::ID)
//			{
//				m_symbolTable[exprLeft.m_token].m_status = asmc::symbolStatus::USED;
//				exprLeft.m_value = m_symbolTable[exprLeft.m_token].m_value;
//			}
//			else
//			{
//				exprLeft.m_value = std::stoi(exprLeft.m_token.m_text);
//			}
//		}
//#pragma endregion
//
//		asmc::ExprVal exprRight;
//		//"<" | "<=" | ">" | ">=" | "==" | "!="
//		while (m_currentToken.m_type == asmc::TokenType::GREATER_THAN 
//			|| m_currentToken.m_type == asmc::TokenType::GREATER_EQ
//			|| m_currentToken.m_type == asmc::TokenType::LESS_THAN
//			|| m_currentToken.m_type == asmc::TokenType::LESS_EQ
//			|| m_currentToken.m_type == asmc::TokenType::EQEQ
//			|| m_currentToken.m_type == asmc::TokenType::NOT_EQ
//			)
//		{
//			//get operator
//			asmc::Token op = m_currentToken;
//
//			moveCurrentToken();
//			exprRight = add_expr();
//
//			checkTable(exprRight);
//
//			//get value, change status of id
//			if (exprRight.m_token.m_type == asmc::TokenType::ID && exprRight.m_recRet == false)
//			{
//				exprRight.m_value = m_symbolTable[exprRight.m_token].m_value;
//				m_symbolTable[exprRight.m_token].m_status = asmc::symbolStatus::USED;
//			}
//
//			if (exprLeft.m_token.m_type == asmc::TokenType::ID)
//			{
//				exprLeft.m_rhsComputed = true;
//			}
//
//
//
//			switch (op.m_type)
//			{
//			case asmc::TokenType::PLUS:
//				exprLeft.m_value += exprRight.m_value;
//				break;
//
//			case asmc::TokenType::MINUS:
//				exprLeft.m_value -= exprRight.m_value;
//				break;
//			}
//
//		}//WHILE END



		//load register
		loadToRegister(exprLeft);

		return exprLeft;
	}

	//<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)* 
	asmc::ExprVal Parser::add_expr()
	{
		asmc::ExprVal exprLeft = mult_expr();				
		

		#pragma region ErrorCheck
		//check id is defined
		if (exprLeft.m_recRet == false)
		{
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
		}
		#pragma endregion

		asmc::ExprVal exprRight;

		while (m_currentToken.m_type == asmc::TokenType::PLUS || m_currentToken.m_type == asmc::TokenType::MINUS)
		{
			//get operator
			asmc::Token op = m_currentToken;
			
			moveCurrentToken();
			exprRight = mult_expr();

			checkTable(exprRight);

			//get value, change status of id
			if (exprRight.m_token.m_type == asmc::TokenType::ID && exprRight.m_recRet == false)
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

		return exprLeft;
	}

	//<mult_expr> ::= <expression> ((* | /) expression )*
	asmc::ExprVal Parser::mult_expr()
	{
		asmc::ExprVal exprLeft = primary();


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

		while (m_currentToken.m_type == asmc::TokenType::ASTERISK || m_currentToken.m_type == asmc::TokenType::SLASH)
		{
			//get operator
			asmc::Token op = m_currentToken;

			moveCurrentToken();
			exprRight = primary();

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

			exprLeft.m_recRet = true;

			switch (op.m_type)
			{
			case asmc::TokenType::ASTERISK:
				exprLeft.m_value *= exprRight.m_value;
				break;

			case asmc::TokenType::SLASH:
				exprLeft.m_value /= exprRight.m_value;
				break;
			}

		}//WHILE END



		//load register
		//loadToRegister(exprLeft);

		return exprLeft;
	}

	//<expression> ::= <id> | <number>
	asmc::ExprVal Parser::primary()
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