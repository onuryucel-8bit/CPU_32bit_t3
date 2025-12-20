#include "Lexer.h"
#include "Parser.h"


int main()
{
	asmc::Lexer lexer("Source.bc");

	asmc::Parser parser(lexer);

	parser.run();
		
}
/*
	---10.12.25---

	LET A		"LET" <ID>
	LET B = 10  "LET" <ID> "=" <number>
	A = A + B   <ID> "=" <ID> + <ID>

	<program> ::= (<let_stmt> | <expression>)*
	<let_stmt> ::= "LET" <ID> ("=" <number>)?
	<expression> ::= <ID> "=" <ID> + <ID>
	<number> ::= [0-9]+


	===================================================
	===================================================
	===================================================
	v0

	<id> ekle
	<expression> => (+ <id> )* kismini duzelt

	LET A = 3
	LET B = 4
	LET C = 5

	A = A + B + C

	LOAD r0,3 //A
	LOAD r1,4 //B
	LOAD r2,5 //C

	ADD r0,r1 //A + B
	ADD r0,r2 //A + B + C

	<program> ::= (<let_stmt> | <expression>)*
	<let_stmt> ::= "LET" <id> ("=" <number>)?
	<expression> ::= <id> "=" <id> (+ <id>)*
	<id> ::= [a-zA-Z]+ 
	<number> ::= [0-9]+

	===================================================
	===================================================
	===================================================
	v1

	<expression> => (+ | -)
				(<id> | <number)


	LET A = 5	  "let" <id> "=" <number>
	LET B = 3	  "let" <id> "=" <number>
	LET C = 1	  "let" <id> "=" <number>

	A = B		  <id> = <id>
	A = 3		  <id> = <number>
	A = B "+ -" C <id> = <id> + <id>
	A = B "+ -" 4 <id> = <id> + <number>
	A = 4 "+ -" B <id> = <number> + <id>

	<program> ::= (<let_stmt> | <expression>)*
	<let_stmt> ::= "LET" <id> ("=" <number>)?
	<expression> ::= <id> "=" (<id> | <number>) ((+ | -) (<id> | <number) )*
	<id> ::= [a-zA-Z]+
	<number> ::= [0-9]+

	===================================================
	===================================================
	===================================================
	v2

	--11.12.25-- carpma hatasi var

	LET a = 10
	LET b = a
	
	a = a * 5 + 2


	<program> ::= (<let_stmt> | <assing_stmt>)*
	<let_stmt> ::= "LET" <id> ("=" <number> | <id>)?
	<assing_stmt> ::= <id> "=" <add_expr>

	<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)*
	<mult_expr> ::= (<id> | <number>) ((* | /) (<id> | <number>))*

	<id> ::= [a-zA-Z]+
	<number> ::= [0-9]+

	===================================================
	===================================================
	===================================================
	v3

	LET a = 10
	LET b = a

	a = (a + 2) * a


	<program> ::= (<let_stmt> | <assing_stmt>)*
	
	<let_stmt> ::= "LET" <id> ("=" <number> | <id>)?


	<assing_stmt> ::= <id> "=" <add_expr>
	
	<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)*
	<mult_expr> ::= <primary> ((* | /) <primary>)*

	<primary> ::= <id>
				| <number>
				| "(" <add_expr> ")"

	<id> ::= [a-zA-Z]+
	<number> ::= [0-9]+

	===================================================
	===================================================
	===================================================
	v4

	LET a = 10
	LET b = 5

	// 10 < 5
	a = a < b

	//C style bool
	//true = [-n,+n]
	//false = 0


	<program> ::= (<let_stmt> | <assing_stmt>)*

	<let_stmt> ::= "LET" <id> ("=" <number> | <id>)?


	<assing_stmt> ::= <id> "=" <expression>

	<expression> ::= <add_expr> (("<" | "<=" | ">" | ">=" | "==" | "!=") <add_expr>)?
	<add_expr> ::= <mult_expr> ((+ | -) <mult_expr>)*
	<mult_expr> ::= <primary> ((* | /) <primary>)*

	<primary> ::= <id>
				| <number>
				| "(" <expression> ")"
	
	<id> ::= [a-zA-Z]+
	<number> ::= [0-9]+

	===================================================
	===================================================
	===================================================
	v5

	LET a = 10
	LET b = 5
	LET c = 0
	LET d = 0

	a = a && b
	//true => a = 1

	a = a && c
	a = b && c
	//false => a = 0

	a = a || b
	a = a || c
	//true 
	
	a = d || c
	//false

*/