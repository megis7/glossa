#include <iostream>

#include "types.h"
#include "Scope.hpp"
#include "AstNode.hpp"
#include "Result.hpp"
#include "Structures.hpp"

using namespace std;

AstNode* root;			// root of the syntax tree

int yyparse();

int main (void) 
{
	auto b = new Boolean(true);		// TODO: if i remove this then all booleans evaluate to false ??? DEBUG

	// setup the local scope of variables
	Scope local;
	Scope::SetCurrentScope(local);
	
	// parse the program
	yyparse ();

	// evaluate the syntax tree
	Result* r = root->Evaluate();

	if(r->IsSane() == false)
		std::cout << "An error has occured: " << *r << std::endl;
	else
		std::cout << "Exited successfully" << std::endl;

	// Debug print all created variables
	cout << "----------------------------" << endl << "Scope: " << endl;
	Scope::GetCurrentScope().PrintAllVariables();

	return 0;	
}

extern "C" void yyerror (char *s) {fprintf (stderr, "%s\n", s);}