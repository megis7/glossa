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
%token LBRAK RBRAK COMMA;

%type <val> atom unary multipl statement statements assignment declaration elseifs while comparison

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

statement:	  expression ';'								{ $<val.node>$ = $<val.node>1; /* This should be removed */ }
			| assignment ';'								{ $<val.node>$ = $<val.node>1; }
			| declaration 									{ $<val.node>$ = $<val.node>1; }
			| conditional									{ $<val.node>$ = $<val.node>1; }
			| while											{ $<val.node>$ = $<val.node>1; }
			;

declaration:  DECLARE INTEGER_DECL IDENTIFIER ';' 			{ $<val.node>$ = new DeclarationNode($<val.sval>3, new Integer(0)); }
			| DECLARE BOOL_DECL IDENTIFIER ';' 				{ $<val.node>$ = new DeclarationNode($<val.sval>3, new Boolean(false)); }
			| DECLARE REAL_DECL IDENTIFIER ';' 				{ $<val.node>$ = new DeclarationNode($<val.sval>3, new Real(0)); }
			;

expression:   comparison					{$<val.node>$ = $<val.node>1;}
			| num_expr						{$<val.node>$ = $<val.node>1;}			
			;

assignment:   IDENTIFIER '=' expression	 	{$<val.node>$ = new AssignmentNode(new IdentifierNode($<val.sval>1), $<val.node>3);}
			;

comparison:   num_expr '<' num_expr			{$<val.node>$ = new LessComparisonNode($<val.node>1, $<val.node>3);}
			;

num_expr:     addition						{$<val.node>$ = $<val.node>1;}
			;

addition: 	  addition '+' multipl			{$<val.node>$ = new AdditionNode($<val.node>1, $<val.node>3);}
			| addition '-' multipl			{$<val.node>$ = new SubtractionNode($<val.node>1, $<val.node>3);}
			| multipl						{$<val.node>$ = $<val.node>1;}
			;

multipl: 	  multipl '*' unary				{$<val.node>$ = new MultiplicationNode($<val.node>1, $<val.node>3);}
		 	| multipl '/' unary				{$<val.node>$ = new DivisionNode($<val.node>1, $<val.node>3);}
 			| unary							{$<val.node>$ = $<val.node>1;}
 			;	

unary:		  '-' atom						{$<val.node>$ = new NegationNode($<val.node>2);}
			| BOOL_NEGATE atom				{$<val.node>$ = new NegationNode($<val.node>2); /* TODO: this must be boolean negation */ }		
			| atom							{$<val.node>$ = $<val.node>1;}
			;

num_expr_list:	  num_expr						{ $<val.node>$ = new NumExprList(); $<val.node>$->AddChild($<val.node>1); }
				| num_expr_list COMMA num_expr 	{ $<val.node>$->AddChild($<val.node>3); }
				;

atom:		  INTEGER									{$<val.node>$ = new LiteralNode(new Integer($<val.ival>1));}
			| REAL										{$<val.node>$ = new LiteralNode(new Real($<val.rval>1));}
			| BOOLEAN									{$<val.node>$ = new LiteralNode(new Boolean($<val.bval>1));}
			| IDENTIFIER								{$<val.node>$ = new IdentifierNode($<val.sval>1);}
			| IDENTIFIER LBRAK num_expr_list RBRAK		{$<val.node>$ = new ArrayNode($<val.sval>1, $<val.node>3);} 
			| '(' expression ')'						{$<val.node>$ = $<val.node>2;}
			;

%%                     /* C code */