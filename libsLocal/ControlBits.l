%option c++
%option lexer=Lexer
%option outfile="Lexer.cpp"
%option header-file="Lexer.h"
%option noyywrap
%{
#include <iostream>
#include <string>

#include "CbTokens.h";

%}

%%



"Read"          return Token{yytext, cb::TokenType::Read};
"Write"         return Token{yytext, cb::TokenType::Write};

"PC_enb"        return Token{yytext, cb::TokenType::PC_enb};
"PC_load"       return Token{yytext, cb::TokenType::PC_load};

"MDR_we" 		return Token{yytext, cb::TokenType::MDR_we};
"MDR_out" 		return Token{yytext, cb::TokenType::MDR_out};
"IR_we" 		return Token{yytext, cb::TokenType::IR_we};
"IR_out" 		return Token{yytext, cb::TokenType::IR_out};
"TEMP_we"		return Token{yytext, cb::TokenType::TEMP_we};
"TEMP_out"		return Token{yytext, cb::TokenType::TEMP_out};
"Posta_we"		return Token{yytext, cb::TokenType::Posta_we};
"Posta_out"		return Token{yytext, cb::TokenType::Posta_out};
"ADR_we"		return Token{yytext, cb::TokenType::ADR_we};
"ADR_out"		return Token{yytext, cb::TokenType::ADR_out};

"I3B"			return Token{yytext, cb::TokenType::I3B};
"MAR_load"		return Token{yytext, cb::TokenType::MAR_load};
"MAR_we"		return Token{yytext, cb::TokenType::MAR_we};
"ACC_we"		return Token{yytext, cb::TokenType::ACC_we};

"FR_cls"		return Token{yytext, cb::TokenType::FR_cls};//flag reg
"ALU_in"		return Token{yytext, cb::TokenType::ALU_in};
"ALU_out"		return Token{yytext, cb::TokenType::ALU_out};

"REG_out"		return Token{yytext, cb::TokenType::REG_out};
"REG_in"		return Token{yytext, cb::TokenType::REG_in};

"@"				return Token{yytext, cb::TokenType::COMMENT};
"{"				return Token{yytext, cb::TokenType::LPAREN};
"}"				return Token{yytext, cb::TokenType::RPAREN};

"#"				return Token{yytext, cb::TokenType::HASH};

[ \t\n]+        { /* skip whitespace */ }
.               { std::cout << "TOKEN: UNKNOWN (" << yytext << ")\n"; }

%%