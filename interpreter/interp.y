%{

extern "C" void yyerror (char *s);
int yylex();

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <functional>

#include <iostream>

#include "AstNode.hpp"
#include "Structures.hpp"

extern AstNode* root;

%}

%union {	
	struct {
		// Type type;		// these are semantics
		union
		{
			int ival;
			bool bval;
			double rval;
			char* sval;
			AstNode* node;
		};
	} val;
}

%token <val> INTEGER
%token <val> BOOLEAN
%token <val> REAL
%token <val> IDENTIFIER

%token DECLARE REAL_DECL INTEGER_DECL BOOL_DECL LESS
%token IF THEN ELSEIF ELSE ENDIF
%token WHILE REPEAT ENDWHILE
%token BOOL_NEGATE

%type <val> number multipl statement statements assignment declaration elseifs while comparison

%%

start:		  statements									{ root = $<val.node>1; }
			;

conditional:	  IF expression THEN statements ENDIF							{ $<val.node>$ = new ConditionalStructure(); $<val.node>$->AddChild(new ConditionalStatements($<val.node>2, $<val.node>4)); }
				| IF expression THEN statements ELSE statements ENDIF 			{ $<val.node>$ = new ConditionalStructure(); $<val.node>$->AddChild(new ConditionalStatements($<val.node>2, $<val.node>4)); $<val.node>$->AddChild(new ConditionalStatements($<val.node>6)); }
				| IF expression THEN statements elseifs ENDIF 					{ $<val.node>$ = $<val.node>5; $<val.node>$->AddChildFront(new ConditionalStatements($<val.node>2, $<val.node>4)); }
				| IF expression THEN statements elseifs ELSE statements ENDIF 	{ $<val.node>$ = $<val.node>5; $<val.node>$->AddChildFront(new ConditionalStatements($<val.node>2, $<val.node>4)); $<val.node>$->AddChild(new ConditionalStatements($<val.node>7)); }
				;

elseifs: 	  ELSEIF expression THEN statements				{ $<val.node>$ = new ConditionalStructure(); $<val.node>$->AddChild(new ConditionalStatements($<val.node>2, $<val.node>4)); }
			| elseifs ELSEIF expression THEN statements		{ $<val.node>$->AddChild(new ConditionalStatements($<val.node>3, $<val.node>5));}
			;

while:		  WHILE expression REPEAT statements ENDWHILE 	{ $<val.node>$ = new IterativeStructure(); $<val.node>$->AddChild(new ConditionalStatements($<val.node>2, $<val.node>4)); }
			;

statements:   %empty										{ $<val.node>$ = new SuccessionStructure(); }
			| statements statement							{ $<val.node>$->AddChild($<val.node>2); }
			;			

statement:	  expression ';'								{ $<val.node>$ = $<val.node>1; }
			| declaration 									{ $<val.node>$ = $<val.node>1; }
			| conditional									{ $<val.node>$ = $<val.node>1;}
			| while											{ $<val.node>$ = $<val.node>1; }
			;

declaration:  DECLARE INTEGER_DECL IDENTIFIER ';' 			{ $<val.node>$ = new DeclarationNode($<val.sval>3, new Integer(0)); }
			| DECLARE BOOL_DECL IDENTIFIER ';' 				{ $<val.node>$ = new DeclarationNode($<val.sval>3, new Boolean(false)); }
			| DECLARE REAL_DECL IDENTIFIER ';' 				{ $<val.node>$ = new DeclarationNode($<val.sval>3, new Real(0)); }
			;

num_expr:     addition						{$<val.node>$ = $<val.node>1;}
			;

expression:   assignment					{$<val.node>$ = $<val.node>1;}
			| comparison					{$<val.node>$ = $<val.node>1;}
			| num_expr						{$<val.node>$ = $<val.node>1;}			
			;

multipl: 	  multipl '*' number			{$<val.node>$ = new MultiplicationNode($<val.node>1, $<val.node>3);}
		 	| multipl '/' number			{$<val.node>$ = new DivisionNode($<val.node>1, $<val.node>3);}
 			| unary							{$<val.node>$ = $<val.node>1;}
 			;	

addition: 	  addition '+' multipl			{$<val.node>$ = new AdditionNode($<val.node>1, $<val.node>3);}
			| addition '-' multipl			{$<val.node>$ = new SubtractionNode($<val.node>1, $<val.node>3);}
			| multipl						{$<val.node>$ = $<val.node>1;}
			;

unary:		  '-' number					{$<val.node>$ = new NegationNode($<val.node>2);}
			| BOOL_NEGATE boolean			{$<val.node>$ = new NegationNode($<val.node>2);}
			| number
			;

comparison:   num_expr '<' num_expr			{$<val.node>$ = new LessComparisonNode($<val.node>1, $<val.node>3);}
			;

number:		  INTEGER						{$<val.node>$ = new LiteralNode(new Integer($<val.ival>1));}
			| REAL							{$<val.node>$ = new LiteralNode(new Real($<val.rval>1));}
			| IDENTIFIER					{$<val.node>$ = new IdentifierNode($<val.sval>1);}
			| '(' num_expr ')'				{$<val.node>$ = $<val.node>2;}
			;

boolean:	  BOOLEAN						{$<val.node>$ = new LiteralNode(new Boolean($<val.bval>1));}
			;

assignment:   IDENTIFIER '=' num_expr	 	{$<val.node>$ = new AssignmentNode(new IdentifierNode($<val.sval>1), $<val.node>3);}
			;

// primary: 	  number					{$$ = $1;}
// 			| bool						{if($1 != 0) $$ = 1; else $$ = 0;}
// 			| '(' expression ')'		{$$ = $2;}
// 			;

// unary:		  '-' unary				{$$ = -$2;}
// 			| primary				{$$ = $1;}
// 			;

// multipl: 	  multipl '*' unary		{$$ = $1 * $3;}
// 			| multipl '/' unary		{$$ = $1 / $3;}
// 			| unary					{$$ = $1;}
// 			;

// addition: 	  addition '+' multipl	{$$ = $1 + $3;}
// 			| addition '-' multipl	{$$ = $1 - $3;}
// 			| multipl				{$$ = $1;}
// 			;

// expression:   addition 						{$$ = $1;}
// 			| '(' assignment ')'  			{$$ = $2;}
// 			;

%%                     /* C code */