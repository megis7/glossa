#include <iostream>

#include "types.h"
#include "Scope.hpp"
#include "AstNode.hpp"
#include "Result.hpp"
#include "Structures.hpp"
#include "types.h"

using namespace std;

AstNode* root;			// root of the syntax tree

// template<typename opType, typename T>
// Result* BinaryOperation(opType op, T a, T b)
// {
//     return (a->*op)(b);
// }

int yyparse();

// class linear : public ValidResult, public Comparable
// {
// public:

// 	virtual Result* operator <(ValidResult* other) { return new Boolean(datum < dynamic_cast<linear*>(other)->datum); }

// 	virtual std::string ToString() const {return "hello";}

// 	virtual Type MyType() const { return INTEGER_TYPE; }
// 	virtual std::string MyTypeString() const {return "Integer";}

// 	virtual Result* operator= (Result * other) { this->datum = ((linear*)other)->datum; }

//     int operator()(int n) const {return n;}
//     int operator[](int n) const {return n * 10;}

// 	int datum;
// };

int main (void) 
{
	auto c = new Boolean(true);		// TODO: if i remove this then all booleans evaluate to false ??? DEBUG

	// ValidResult* x = new Integer(1);
	// ValidResult* y = new Real(2);

	// linear a, b;
	// a.datum = 10;
	// b.datum = 9;

    // std::cout << *(operation(&linear::operator<, &a, &b)) << std::endl;
    // std::cout << *(BinaryOperation(&Operable::operator+, x, y)) << std::endl;
    // std::cout << *(operation(&Integer::operator<, x, y)) << std::endl;

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