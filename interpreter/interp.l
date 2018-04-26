%{

#include "types.h"
#include "AstNode.hpp"
#include "y.tab.h"
extern "C" void yyerror(char* str);

%}
%%

"!".*                   ;

"declare"               {return DECLARE;}
"integer"               {return INTEGER_DECL;}
"real"                  {return REAL_DECL;}
"boolean"               {return BOOL_DECL;}

"if"                    {return IF;}
"then"                  {return THEN;}
"else"                  {return ELSE;}
"elseif"                {return ELSEIF;}
"endif"                 {return ENDIF;}

"while"                 {return WHILE;}
"repeat"                {return REPEAT;}
"endwhile"              {return ENDWHILE;}

"["                     { return LBRAK; }
"]"                     { return RBRAK; }
","                     { return COMMA; }

"not"                   {return BOOL_NEGATE;}

"true"                  {yylval.val.bval = 1; return BOOLEAN;}
"false"                 {yylval.val.bval = 0; return BOOLEAN;}

[a-zA-Z]+			    {yylval.val.sval = strcpy(new char[strlen(yytext)], yytext); return IDENTIFIER;}

[0-9]+                  {yylval.val.ival = atoi(yytext); return INTEGER;}
([0-9]+[.][0-9]+)       {yylval.val.rval = atof(yytext); return REAL;}

[ \t\n]                 ;                                
[-+=*/!()<;]            {return yytext[0];}
.                       {ECHO; yyerror ("unexpected character");}

%%
int yywrap (void) {return 1;}