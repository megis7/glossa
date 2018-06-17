%{

extern "C" void yyerror (char *s);
int yylex();

#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>

#include <iostream>

#include <AstNode.hpp>
#include <Structures.hpp>

extern AstNode* root;

%}

%union {	
	struct {
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
%token <val> STRING
%token <val> IDENTIFIER

%token DECLARE REAL_DECL INTEGER_DECL BOOL_DECL STRING_DECL
%token IF THEN ELSEIF ELSE ENDIF
%token WHILE REPEAT ENDWHILE
%token BOOL_NEGATE
%token LBRAK RBRAK COMMA
%token LESS LESS_THAN GREATER GREATER_THAN
%token AND OR
%token DEQUALS NEQUALS
%token ASSIGN

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

statement:	  expression ';'								{ $<val.node>$ = $<val.node>1; }
			| assignment ';'								{ $<val.node>$ = $<val.node>1; }
			| declaration ';'								{ $<val.node>$ = $<val.node>1; }
			| conditional									{ $<val.node>$ = $<val.node>1; }
			| while											{ $<val.node>$ = $<val.node>1; }
			;

declaration:  DECLARE INTEGER_DECL identifier_list 		{ $<val.node>$ = new DeclarationNode<Integer>($<val.node>3); }
			| DECLARE BOOL_DECL identifier_list 		{ $<val.node>$ = new DeclarationNode<Boolean>($<val.node>3); }
			| DECLARE REAL_DECL identifier_list 		{ $<val.node>$ = new DeclarationNode<Real>($<val.node>3); }
			| DECLARE STRING_DECL identifier_list 		{ $<val.node>$ = new DeclarationNode<String>($<val.node>3); }
			;

identifier_list:	  IDENTIFIER								{ $<val.node>$ = new IdentifierList($<val.sval>1); }
					| identifier_list COMMA IDENTIFIER			{ ((IdentifierList*)$<val.node>$)->AddIdentifier($<val.sval>3); }
					;

expression:   lcomparison							{$<val.node>$ = $<val.node>1;}			
			;

assignment:   IDENTIFIER ASSIGN expression	 		{$<val.node>$ = new AssignmentNode(new IdentifierNode($<val.sval>1), $<val.node>3);}
			;


lcomparison:  lcomparison AND equation				{$<val.node>$ = new ANDComparisonNode($<val.node>1, $<val.node>3);}
			| lcomparison OR equation				{$<val.node>$ = new ORComparisonNode($<val.node>1, $<val.node>3);}
			| equation								{$<val.node>$ = $<val.node>1;}
			;	

equation:	  comparison DEQUALS comparison			{$<val.node>$ = new EqualComparisonNode($<val.node>1, $<val.node>3);}
			| comparison NEQUALS comparison			{$<val.node>$ = new NotEqualComparisonNode($<val.node>1, $<val.node>3);}
			| comparison							{$<val.node>$ = $<val.node>1;}
			;

comparison:   num_expr LESS num_expr				{$<val.node>$ = new LessComparisonNode($<val.node>1, $<val.node>3);}
			| num_expr GREATER num_expr				{$<val.node>$ = new GreaterComparisonNode($<val.node>1, $<val.node>3);}
			| num_expr LESS_THAN num_expr			{$<val.node>$ = new LessThanComparisonNode($<val.node>1, $<val.node>3);}
			| num_expr GREATER_THAN num_expr		{$<val.node>$ = new GreaterThanComparisonNode($<val.node>1, $<val.node>3);}
			| num_expr								{$<val.node>$ = $<val.node>1;}
			;

num_expr:     addition						{$<val.node>$ = $<val.node>1;}
			;

// addition: 	  addition '+' multipl			{$<val.node>$ = new AdditionNode($<val.node>1, $<val.node>3);}
addition: 	  addition '+' multipl			{$<val.node>$ = new BinaryNodeSpecific<const ValidResult*, Operable>($<val.node>1, $<val.node>3, &Operable::operator+);}
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
			| STRING									{$<val.node>$ = new LiteralNode(new String($<val.sval>1));}
			| IDENTIFIER								{$<val.node>$ = new IdentifierNode($<val.sval>1);}
			| IDENTIFIER LBRAK num_expr_list RBRAK		{$<val.node>$ = new ArrayNode($<val.sval>1, $<val.node>3);} 
			| '(' expression ')'						{$<val.node>$ = $<val.node>2;}
			;

%%                     /* C code */