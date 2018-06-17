#include <iostream>

#include <types.h>
#include <Scope.hpp>
#include <AstNode.hpp>
#include <Result.hpp>
#include <Structures.hpp>

using namespace std;

AstNode* root;			// root of the syntax tree

int yyparse();

int main (void) 
{
	// Result* (Operable::*x)(const ValidResult*) = &Operable::operator+;

	Integer* x1 = new Integer(5);
	Integer* x2 = new Integer(6);

	// Result* res = (x1->*x)(x2);

	// std::cout << *res << std::endl;

	// auto b = new BinaryNodeSpecific<const ValidResult*, Operable>(new LiteralNode(x1), new LiteralNode(x2), &Operable::operator+);

	// std::cout << *b->Evaluate() << std::endl;

	// // std::cout << ((new Integer(1))->*x(new Integer(10))) << std::endl;
	// return 0;

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