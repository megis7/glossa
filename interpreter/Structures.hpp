#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "AstNode.hpp"
#include "Result.hpp"

template <class T>
bool TryConvert(Result* x, T* &y)
{
	y = dynamic_cast<T*>(x);
	return y != nullptr; 
}

class SuccessionStructure : public AstNode
{
public:
	virtual Result* Evaluate();	
};

class ConditionalStructure : public AstNode
{
public:
	virtual Result* Evaluate();
};

class IterativeStructure : public AstNode
{
public:
	virtual Result* Evaluate();
};

class ConditionalStatements : public AstNode
{
public:
	ConditionalStatements() : condition(nullptr), statements(nullptr) {}
	ConditionalStatements(AstNode* _condition, AstNode* _statements) : condition(_condition) {statements = dynamic_cast<SuccessionStructure*>(_statements);}
	ConditionalStatements(AstNode* _statements) : condition(new LiteralNode(&Boolean::BooleanTrue)) {statements = dynamic_cast<SuccessionStructure*>(_statements);}
	virtual Result* Evaluate();

private:
	AstNode* condition;
	SuccessionStructure* statements;
};

#endif