#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <vector>
#include <string>

#include "Result.h"

class AstNode
{
public:
	virtual Result* Evaluate() = 0;

	void AddChild(AstNode* child);
	void AddChildFront(AstNode* child);
	bool IsLeaf();

	int ChildrenCount() {return children.size(); }

	Result* HandleError(ErrorResult* error);

	virtual ~AstNode();

protected:
	std::vector<AstNode*> children;
};

class BinaryNode : public AstNode
{
protected:
	BinaryNode(){}
	BinaryNode(AstNode* lhs, AstNode* rhs) { AddChild(lhs); AddChild(rhs); } 

	std::pair<Result*, Result*> GetBinaryOperands();
};

class AdditionNode : public BinaryNode
{
public:
	AdditionNode() {}
	AdditionNode(AstNode* lhs, AstNode* rhs) : BinaryNode(lhs, rhs) {}

	virtual Result* Evaluate();

};

class MultiplicationNode : public BinaryNode
{
public:
	MultiplicationNode() {}
	MultiplicationNode(AstNode* lhs, AstNode* rhs) : BinaryNode(lhs, rhs) {}

	virtual Result* Evaluate();

};

class DivisionNode : public BinaryNode
{
public:
	DivisionNode() {}
	DivisionNode(AstNode* lhs, AstNode* rhs) : BinaryNode(lhs, rhs) {}

	virtual Result* Evaluate();

};

class LiteralNode : public AstNode
{
public:
	LiteralNode() : datum(nullptr) {}
	LiteralNode(Result* _datum) : datum(_datum) {}

	virtual Result* Evaluate();

	inline void SetToken(Result* newToken) { datum = newToken; }
	inline Result* GetToken() { return datum; }

private:
	Result* datum;
};

class IdentifierNode : public AstNode
{
public:
	IdentifierNode() : name("") {}
	IdentifierNode(std::string _name) : name(_name) {}

	virtual Result* Evaluate();
	std::string GetIdentifierName() {return name;}

private:
	std::string name;				// identifier name
};

class StatementsNode : public AstNode
{
public:
	virtual Result* Evaluate();	
};

class AssignmentNode : public BinaryNode
{
public:
	AssignmentNode() {}
	AssignmentNode(AstNode* identifierNode, AstNode* expression) : BinaryNode(identifierNode, expression) {}

	virtual Result* Evaluate();
};

class ConditionalStatements : public AstNode
{
public:
	ConditionalStatements() : condition(nullptr), statements(nullptr) {}
	ConditionalStatements(AstNode* _condition, AstNode* _statements) : condition(_condition) {statements = dynamic_cast<StatementsNode*>(_statements);}
	ConditionalStatements(AstNode* _statements) : condition(new LiteralNode(&Boolean::BooleanTrue)) {statements = dynamic_cast<StatementsNode*>(_statements);}
	virtual Result* Evaluate();

private:
	AstNode* condition;
	StatementsNode* statements;
};

class ConditionalStructure : public AstNode
{
public:
	virtual Result* Evaluate();
};

class DeclarationNode : public AstNode
{
public:
	DeclarationNode() : identifierName(""), identifierObject(nullptr) {}
	DeclarationNode(std::string name, ValidResult* value) : identifierName(name), identifierObject(value) {}
	
	virtual Result* Evaluate();
private:
	std::string identifierName;
	ValidResult* identifierObject; 
};

class IterativeStructure : public AstNode
{
public:
	virtual Result* Evaluate();
};

#endif