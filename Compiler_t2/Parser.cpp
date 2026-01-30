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

		m_labelIndex = 0;

		m_file.open("main.asm");
		
		if (!m_file.is_open())
		{
			printError("failed to create main.asm");			
		}
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
			m_file << str << "\n";
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
	
		std::cout << "Parser running... \n"
				  << "===========================\n";

		if (m_lexer.getErrorFlag() == false)
		{
			
			//std::cout << m_currentToken.m_text << "\n";
			program();
			
		}
		std::cout << "===========================\n"
				  << "Printing VAR-SECTION...\n";

		emit(".origin 0x1000");		
		/*for (variableVec& vec : m_varlist)
		{
			std::cout << "data: " << (char)vec.m_value  << " " << vec.m_ramIndex << " = ";
			emit(".db " + std::to_string(vec.m_value));
		}*/

		
		m_file << ".db ";
		for (size_t i = 0; i < m_varlist.size(); ++i)
		{
			m_file <<"0x"<< std::hex << m_varlist[i].m_value;
			if (i + 1 < m_varlist.size())
			{
				m_file << ",";
			}
		}
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
	<program> ::= <let_stmt> 
				| <assing_stmt> 
				| <print_stmt>
				| <if_stmt>
				| <while_stmt>
				| <goto_stmt>
				| <label_stmt>

	<while_stmt> ::= "WHILE" "THEN" "END"
	
	<if_stmt> ::= "IF" "THEN" "END"

	<print_stmt> ::= PRINT "(" <id> ")" ";"

	<let_stmt> ::= "LET" <id> "=" <expression> ";"	

	<expression> ::= <add_expr> (("<" | "<=" | ">" | ">=" | "==" | "!=") <add_expr>)?
	<assing_stmt> ::= <id> "=" <add_expr>* ";"
	<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)*
	<mult_expr> ::= <primary> ((* | /) <primary>)*
	<primary> ::= <id> | <number>

	<id> ::= [a-z]+
	<number> ::= [0-9]+

	*/
	void Parser::program()
	{
		while (m_currentToken.m_type != asmc::TokenType::ENDOFFILE)
		{
			statements();
		}				
	}

	void Parser::statements()
	{
		if (m_currentToken.m_type == asmc::TokenType::LET	||
			m_currentToken.m_type == asmc::TokenType::ID	||
			m_currentToken.m_type == asmc::TokenType::PRINT ||
			m_currentToken.m_type == asmc::TokenType::IF    ||
			m_currentToken.m_type == asmc::TokenType::WHILE ||			
			m_currentToken.m_type == asmc::TokenType::BUZZ	||
			m_currentToken.m_type == asmc::TokenType::DRAW)


		{

			switch (m_currentToken.m_type)
			{
			case asmc::TokenType::DRAW:
				draw_stmt();
				break;

			case asmc::TokenType::BUZZ:
				buzz_stmt();
				break;			

			case asmc::TokenType::WHILE:
				while_stmt();
				break;

			case asmc::TokenType::IF:
				if_stmt();
				break;

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

	void Parser::while_stmt()
	{
		match(asmc::TokenType::WHILE);
		asmc::ExprVal exprLeft = primary();

		size_t loopStartPos = m_labelIndex;
		emit("l" + std::to_string(loopStartPos) + ":");

		//emit("....");
		if (exprLeft.m_token.m_type == asmc::TokenType::ID)
		{
			checkTable(exprLeft);
			emit("LOAD r0,@" + asmc_getAdr_asString(exprLeft));
		}
		//number
		else
		{
			//TODO register release
			emit("LOAD r0,0x" + std::to_string(exprLeft.m_value));
		}

		m_labelIndex++;
		size_t currentLabelIndex = m_labelIndex;

		asmc::ExprVal exprRight;
		//"<" | "<=" | ">" | ">=" | "==" | "!="
		if (m_currentToken.m_type == asmc::TokenType::GREATER_THAN
			|| m_currentToken.m_type == asmc::TokenType::GREATER_EQ
			|| m_currentToken.m_type == asmc::TokenType::LESS_THAN
			|| m_currentToken.m_type == asmc::TokenType::LESS_EQ
			|| m_currentToken.m_type == asmc::TokenType::EQEQ
			|| m_currentToken.m_type == asmc::TokenType::NOT_EQ
			)
		{

			asmc::Token op = m_currentToken;
			moveCurrentToken();
			exprRight = primary();

			checkTable(exprRight);

			if (exprRight.m_token.m_type == asmc::TokenType::ID)
			{
				emit("LOAD r1,@" + asmc_getAdr_asString(exprRight));
			}
			else
			{
				emit("LOAD r1,0x" + rdx::decToHex(exprRight.m_value));
			}

			switch (op.m_type)
			{
			case asmc::TokenType::GREATER_THAN:
				emit("CMP r0, r1");
				emit("JL l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::GREATER_EQ:
				emit("CMP r0, r1");
				emit("JL l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::LESS_THAN:
				emit("CMP r0, r1");
				emit("JG l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::LESS_EQ:
				emit("CMP r0, r1");
				emit("JG l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::NOT_EQ:
				emit("CMP r0, r1");
				emit("JE l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::EQEQ:
				emit("CMP r0, r1");
				emit("JNE l" + std::to_string(currentLabelIndex));
				break;

			}

			//emit("....");
			match(asmc::TokenType::THEN);
			m_labelIndex++;
			while (m_currentToken.m_type != asmc::TokenType::END && m_currentToken.m_type != asmc::TokenType::ENDOFFILE)
			{
				statements();
			}
		}

		
		emit("JMP l" + std::to_string(loopStartPos));
		emit("l" + std::to_string(currentLabelIndex) + ":");


		match(asmc::TokenType::END);
		
	}

	//<if_stmt> ::= IF <id> "!=" <number> THEN <statments> END
	void Parser::if_stmt()
	{
		match(asmc::TokenType::IF);

		asmc::ExprVal exprLeft = primary();
		//emit("....");
		if (exprLeft.m_token.m_type == asmc::TokenType::ID)
		{
			checkTable(exprLeft);						
			emit("LOAD r0,@" + asmc_getAdr_asString(exprLeft));
		}
		//number
		else
		{
			//TODO register release
			emit("LOAD r0,0x" + std::to_string(exprLeft.m_value));
		}
		
		size_t currentLabelIndex = m_labelIndex;

		asmc::ExprVal exprRight;
		//"<" | "<=" | ">" | ">=" | "==" | "!="
		if (m_currentToken.m_type == asmc::TokenType::GREATER_THAN
			|| m_currentToken.m_type == asmc::TokenType::GREATER_EQ
			|| m_currentToken.m_type == asmc::TokenType::LESS_THAN
			|| m_currentToken.m_type == asmc::TokenType::LESS_EQ
			|| m_currentToken.m_type == asmc::TokenType::EQEQ
			|| m_currentToken.m_type == asmc::TokenType::NOT_EQ
			)
		{

			asmc::Token op = m_currentToken;
			moveCurrentToken();
			exprRight = primary();

			checkTable(exprRight);
			
			if (exprRight.m_token.m_type == asmc::TokenType::ID)
			{
				emit("LOAD r1,@" + asmc_getAdr_asString(exprRight));
			}
			else
			{
				emit("LOAD r1,0x" + rdx::decToHex(exprRight.m_value));
			}
			
			switch (op.m_type)
			{
			case asmc::TokenType::GREATER_THAN:
				emit("CMP r0, r1");
				emit("JL l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::GREATER_EQ:
				emit("CMP r0, r1");
				emit("JL l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::LESS_THAN:
				emit("CMP r0, r1");
				emit("JG l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::LESS_EQ:
				emit("CMP r0, r1");
				emit("JG l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::NOT_EQ:				
				emit("CMP r0, r1");
				emit("JE l" + std::to_string(currentLabelIndex));
				break;

			case asmc::TokenType::EQEQ:
				emit("CMP r0, r1");
				emit("JNE l" + std::to_string(currentLabelIndex));
				break;
										
			}

			//emit("....");
			match(asmc::TokenType::THEN);
			m_labelIndex++;
			while (m_currentToken.m_type != asmc::TokenType::END && m_currentToken.m_type != asmc::TokenType::ENDOFFILE)
			{
				statements();
			}
		}
		


		emit("l" + std::to_string(currentLabelIndex) + ":");
			

		match(asmc::TokenType::END);
	}

	//"LET" <id> "=" <expression> ";"
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
		
			//STRING
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

				
				ramIndex = m_memManager.allocVariable();

				varvec = { .m_value = '\0' ,.m_ramIndex = ramIndex};
				m_varlist.push_back(varvec);


				match(asmc::TokenType::STRING);
			}
			//INT
			//LET a = 10;
			else
			{
				asmc::ExprVal value = primary();

				uint32_t entryVal;

				if (value.m_token.m_type == asmc::TokenType::CHAR)
				{
					entryVal = value.m_token.m_text[0];
				}
				else
				{
					entryVal = std::stoi(value.m_token.m_text);
				}
				

				

				uint32_t ramIndex = m_memManager.allocVariable();

				//insert variable to the table
				m_symbolTable[idName.m_token].m_value = entryVal;
				m_symbolTable[idName.m_token].m_ramIndex = ramIndex;
				
				asmc::variableVec varvec = { .m_value = entryVal ,.m_ramIndex = ramIndex };
				m_varlist.push_back(varvec);
			}														
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

		
		std::string sourceAdr;
		

		switch (m_symbolTable[exprId.m_token].m_type)
		{
		case asmc::variableType::Int32:
								
				sourceAdr = asmc_getAdr_asString(exprId);

				emit("LOAD r0,@" + sourceAdr);
				emit("STR @" + rdx::decToHex(asmc_TTY_adr) + ",r0");

			break;

		case asmc::variableType::String:
												
			
			///*
			//	int i = 0;
			//	char ch = str[i];
			// 
			//	while(str[i] != '\0')
			//	{
			//		std::cout << ch;
			//		i++;
			//		ch = str[i]
			//	}
			emit("LOAD r0,0x" + asmc_getAdr_asString(exprId));
			emit("LOAD r1,0x" + rdx::decToHex(m_symbolTable[exprId.m_token].m_value));
			emit("l" + std::to_string(m_labelIndex) + ": ");
				emit("STR @" + rdx::decToHex(asmc_TTY_adr) + ",r1");
				emit("ADD r0, 0x1");
				emit("LOAD r1, @r0");
				emit("CMP r1, 0x0");
				emit("JNE l" + std::to_string(m_labelIndex));

				m_labelIndex++;
			break;
		}
			
		match(asmc::TokenType::RPAREN);
		match(asmc::TokenType::SEMICOLON);
			
	}

	//<buzz_stmt> ::= BUZZ (<id> | <number>) (0 | 1) ;
	void Parser::buzz_stmt()
	{
		match(asmc::TokenType::BUZZ);
		match(asmc::TokenType::LPAREN);
		

		asmc::ExprVal expr = primary();
		match(asmc::TokenType::COMMA);

		//TODO char uhh... (?_?)
		if (expr.m_token.m_type == asmc::TokenType::CHAR)
		{
			printError("expected id or number found char ");
		}

		checkTable(expr);
		
		asmc::ExprVal type = number();

		switch (expr.m_token.m_type)
		{

		case asmc::TokenType::ID:
			emit("LOAD r0, @" + asmc_getAdr_asString(expr));			
			break;
		
		case asmc::TokenType::NUMBER:
			emit("LOAD r0, 0x" + std::to_string(expr.m_value));					
			break;
		}

		if (type.m_value == 0)
		{
			emit("STR @" + rdx::decToHex(asmc_BUZZER_adr) + ",r0");			
		}
		else
		{
			emit("STR @" + rdx::decToHex(asmc_BUZZER_con) + ",r0");
		}

		match(asmc::TokenType::RPAREN);
		match(asmc::TokenType::SEMICOLON);

	}
	//<draw_stmt> ::= (<id> | <number>) (<id> | <number>) (<id> | <number>)
	void Parser::draw_stmt()
	{
		match(asmc::TokenType::DRAW);

		match(asmc::TokenType::LPAREN);

		asmc::ExprVal posx = primary();
		match(asmc::TokenType::COMMA);

		asmc::ExprVal posy = primary();
		match(asmc::TokenType::COMMA);

		asmc::ExprVal color = primary();		
		
		checkTable(posx);
		checkTable(posy);
		
		/*
			0001 -> drawPixel
			0000 -> color
			0000'0000 -> posx
			0000'0000 -> posy

			posy		posx		color  command
			0(000'0000) 0(000'0000) 0(000) 0(001)
		
		*/

		//Posx
		if (posx.m_token.m_type == asmc::TokenType::ID)
		{
			emit("LOAD r0, @" + asmc_getAdr_asString(posx));
		}
		else
		{
			emit("LOAD r0, 0x" + std::to_string(posx.m_value));
		}
		
		emit("STR @" + rdx::decToHex(asmc_POSX_adr) + ",r0");

		//Posy

		if (posy.m_token.m_type == asmc::TokenType::ID)
		{
			emit("LOAD r0, @" + asmc_getAdr_asString(posy));
		}
		else
		{
			emit("LOAD r0, 0x" + std::to_string(posy.m_value));
		}
		emit("STR @" + rdx::decToHex(asmc_POSY_adr) + ",r0");

		//Color
		if (color.m_token.m_type == asmc::TokenType::ID)
		{
			emit("LOAD r0,0x" + rdx::decToHex(color.m_value));
		}
		else
		{
			emit("LOAD r0, 0x" + std::to_string(color.m_value));
		}
		emit("STR @" + rdx::decToHex(asmc_COLOR_adr) + ",r0");					

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

		asmc::ExprVal result = add_expr();

		//constantFolding();
		
		//------------------//

		if (result.m_location == asmc::Location::None)
		{
			emit("STR @" + asmc_getAdr_asString(idRes) + ",0x" + rdx::decToHex(result.m_value));
		}
		else
		{
			emit("STR @" + asmc_getAdr_asString(idRes) + ",r" + std::to_string((int)result.m_registerIndex));
		}

		//m_symbolTable[idRes.m_token].m_value = result.m_value;

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
		asmc::ExprVal exprRight;
		//"<" | "<=" | ">" | ">=" | "==" | "!="
		while (m_currentToken.m_type == asmc::TokenType::GREATER_THAN
			|| m_currentToken.m_type == asmc::TokenType::GREATER_EQ
			|| m_currentToken.m_type == asmc::TokenType::LESS_THAN
			|| m_currentToken.m_type == asmc::TokenType::LESS_EQ
			|| m_currentToken.m_type == asmc::TokenType::EQEQ
			|| m_currentToken.m_type == asmc::TokenType::NOT_EQ
			)
		{

			asmc::Token op = m_currentToken;
			moveCurrentToken();
			exprRight = add_expr();

			switch (op.m_type)
			{
			case asmc::TokenType::LESS_THAN:
				exprLeft.m_value = exprLeft.m_value < exprRight.m_value ? 1 : 0;
				emit("JLT l" + std::to_string(m_labelIndex));
				break;
						
			}
		}
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
		//loadToRegister(exprLeft);

		return exprLeft;
	}

	//<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)* 
	asmc::ExprVal Parser::add_expr()
	{
		asmc::ExprVal exprLeft = mult_expr();		

		if (exprLeft.m_token.m_type == asmc::TokenType::ID)
		{
			//exprLeft.m_value = std::stoi(exprLeft.m_token.m_text);			
		
			loadToRegister(exprLeft);
			
			exprLeft.m_value = m_symbolTable[exprLeft.m_token].m_value;
			m_symbolTable[exprLeft.m_token].m_status = asmc::symbolStatus::USED;			
		}

		asmc::ExprVal exprRight;

		while (m_currentToken.m_type == asmc::TokenType::PLUS || m_currentToken.m_type == asmc::TokenType::MINUS)
		{
			//get operator
			asmc::Token op = m_currentToken;						
			
			moveCurrentToken();
			exprRight = mult_expr();

			checkTable(exprRight);		
			
			//<id> + (<id> | <number>)
			if (exprLeft.m_token.m_type == asmc::TokenType::ID)
			{			
				if (exprRight.m_token.m_type == asmc::TokenType::ID)
				{										
					loadToRegister(exprRight);

					switch (op.m_type)
					{
					case asmc::TokenType::PLUS:
						emit("ADD r" + std::to_string(exprLeft.m_registerIndex) + ", r1");
						break;

					case asmc::TokenType::MINUS:
						emit("SUB r" + std::to_string(exprLeft.m_registerIndex) + ", r1");
						break;
					}

					m_memManager.releaseRegister(exprRight.m_registerIndex);
				}
				else
				{
					switch (op.m_type)
					{
					case asmc::TokenType::PLUS:
						emit("add r" + std::to_string(exprLeft.m_registerIndex) + "," + std::to_string(exprRight.m_value));
						break;

					case asmc::TokenType::MINUS:						
						emit("sub r" + std::to_string(exprLeft.m_registerIndex) + "," + std::to_string(exprRight.m_value));
						break;
					}
				}				
			}
			//<number> + <number>
			else if (exprRight.m_token.m_type == asmc::TokenType::NUMBER)
			{				
				switch (op.m_type)
				{
					case asmc::TokenType::PLUS:
						exprLeft.m_value += exprRight.m_value;
						break;

					case asmc::TokenType::MINUS:
						exprLeft.m_value -= exprRight.m_value;
						break;
				}
			}
			//<number> + <id>
			else if (exprRight.m_token.m_type == asmc::TokenType::ID)
			{				
				switch (op.m_type)
				{
					case asmc::TokenType::PLUS:
				
						loadToRegister(exprRight);
						emit("ADD r"+ std::to_string(exprRight.m_registerIndex) + "," + std::to_string(exprLeft.m_value));

						exprLeft = exprRight;
						break;

					case asmc::TokenType::MINUS:
						loadToRegister(exprRight);
						emit("SUB r" + std::to_string(exprRight.m_registerIndex) + "," + std::to_string(exprLeft.m_value));

						exprLeft = exprRight;
						break;
				}
			}


			////get value, change status of id
			//if (exprRight.m_token.m_type == asmc::TokenType::ID)
			//{
			//	exprRight.m_value = m_symbolTable[exprRight.m_token].m_value;
			//	m_symbolTable[exprRight.m_token].m_status = asmc::symbolStatus::USED;

			//	loadToRegister(exprRight);
			//}
			//
			//
			//if(exprLeft.m_token.m_type == asmc::TokenType::NUMBER &&
			//	exprRight.m_token.m_type == asmc::TokenType::NUMBER)
			//{
			//	switch (op.m_type)
			//	{
			//	case asmc::TokenType::PLUS:
			//		exprLeft.m_value += exprRight.m_value;
			//		break;

			//	case asmc::TokenType::MINUS:
			//		exprLeft.m_value -= exprRight.m_value;
			//		break;
			//	}
			//}			
			//else
			//{
			//	switch (op.m_type)
			//	{
			//	case asmc::TokenType::PLUS:

			//		emit("ADD r" + std::to_string(exprLeft.m_registerIndex) + ", 0x" + std::to_string(exprRight.m_value));
			//		break;

			//	case asmc::TokenType::MINUS:
			//		emit("SUB r" + std::to_string(exprLeft.m_registerIndex) + ", 0x" + std::to_string(exprRight.m_value));
			//		break;
			//	}
			//}
			

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
			loadToRegister(exprLeft);
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

			//<id> + (<id> | <number>)
			if (exprLeft.m_token.m_type == asmc::TokenType::ID)
			{
				if (exprRight.m_token.m_type == asmc::TokenType::ID)
				{				
					loadToRegister(exprRight);

					switch (op.m_type)
					{
					case asmc::TokenType::ASTERISK:
						emit("MUL r" + std::to_string(exprLeft.m_registerIndex) + ", r1");
						break;

					case asmc::TokenType::SLASH:
						emit("DIV r" + std::to_string(exprLeft.m_registerIndex) + ", r1");
						break;
					}

					m_memManager.releaseRegister(exprRight.m_registerIndex);
				}
				else
				{
					switch (op.m_type)
					{
					case asmc::TokenType::ASTERISK:
						emit("MUL r" + std::to_string(exprLeft.m_registerIndex) + "," + std::to_string(exprRight.m_value));
						break;

					case asmc::TokenType::SLASH:
						emit("DIV r" + std::to_string(exprLeft.m_registerIndex) + "," + std::to_string(exprRight.m_value));
						break;
					}
				}
			}
			//<number> + <number>
			else if (exprRight.m_token.m_type == asmc::TokenType::NUMBER)
			{
				switch (op.m_type)
				{
				case asmc::TokenType::ASTERISK:
					exprLeft.m_value *= exprRight.m_value;
					break;

				case asmc::TokenType::SLASH:
					exprLeft.m_value /= exprRight.m_value;
					break;
				}
			}
			//<number> + <id>
			else if (exprRight.m_token.m_type == asmc::TokenType::ID)
			{
				switch (op.m_type)
				{
				case asmc::TokenType::ASTERISK:
					loadToRegister(exprRight);
					emit("MUL r" + std::to_string(exprRight.m_registerIndex) + "," + std::to_string(exprLeft.m_value));

					exprLeft = exprRight;
					break;

				case asmc::TokenType::SLASH:
					loadToRegister(exprRight);
					emit("DIV r" + std::to_string(exprRight.m_registerIndex) + "," + std::to_string(exprLeft.m_value));

					exprLeft = exprRight;
					break;
				}
			}

		}//WHILE END



		//load register
		//loadToRegister(exprLeft);

		return exprLeft;
	}

	//<primary> ::= <id> | <number> | <char>
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
		else if (m_currentToken.m_type == asmc::TokenType::CHAR)
		{
			retval = charType();
		}
		else
		{
			printError("id or number expected at");
		}

		
		return retval;
	}

	//<char>
	asmc::ExprVal Parser::charType()
	{
		asmc::ExprVal retval;

		if (m_currentToken.m_type == asmc::TokenType::CHAR)
		{
			retval.m_token = m_currentToken;
		}
		else
		{
			printError("expected <CHAR>, found");
		}

		moveCurrentToken();

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

	

	

	//TODO return bool
	void Parser::checkTable(const asmc::ExprVal& expval)
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
			printError("expected [" + std::string(magic_enum::enum_name(type)) + "] found [" + m_currentToken.m_text +"]");

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

		if (expval.m_token.m_type == asmc::TokenType::ID)
		{
			emit("LOAD r" + std::to_string(expval.m_registerIndex) + ",@" + std::to_string(m_symbolTable[expval.m_token].m_ramIndex));
		}
		//asmc::TokenType::NUMBER
		else
		{
			emit("LOAD r" + std::to_string(expval.m_registerIndex) + ",0x" + rdx::decToHex(expval.m_value));
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

		case asmc::TokenType::DIV:
			opcode = "MUL";
			break;
		}

		emit(opcode + " r" + std::to_string(registerIndex) + ", " + type + std::to_string(value));
			
	}

}