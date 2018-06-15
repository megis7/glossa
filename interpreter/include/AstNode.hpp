#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cassert>

#include <Result.hpp>

class AstNode
{
public:
	virtual Result* Evaluate() = 0;

	void AddChild(AstNode* child);
	std::vector<AstNode*> GetChildren() {return children;}
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

	// Extracts binary operands from the 'children' array and applies the given operator by calling the 'BinaryOperation' function
	template<typename P, typename iType> Result* Apply(Result* (iType::*operation) (P) )
	{
		assert(children.size() == 2);		// Assert binary node has valid children count

		Result *lhs, *rhs;

		if ((lhs = children[0]->Evaluate())->IsSane() == false)
			return AstNode::HandleError(Convert<ErrorResult>(lhs));

		if((rhs = children[1]->Evaluate())->IsSane() == false)
			return AstNode::HandleError(Convert<ErrorResult>(rhs));

		iType* token_l;
		if(TryConvert<Result, iType>(lhs, token_l) == false)
			return new ErrorResult("Operand " + lhs->MyTypeString() + " is not of " + iType::ToString() + " type");

		ValidResult* token_r = (ValidResult*)(rhs);

		Result* result = BinaryOperation(operation, token_l, token_r);

		return result;
	}

private:
	// Applies the given operator over the given operands by using polymorphism on the left-hand-side operand 
	template<typename opType, typename X, typename Y>
	static Result* BinaryOperation(opType op, X lhs, Y rhs)
	{
		return (lhs->*op)(rhs);
	}
};

class AdditionNode : public BinaryNode
{
public:
	AdditionNode() {}
	AdditionNode(AstNode* lhs, AstNode* rhs) : BinaryNode(lhs, rhs) {}

	virtual Result* Evaluate();

};

class SubtractionNode : public BinaryNode
{
public:
	SubtractionNode() {}
	SubtractionNode(AstNode* lhs, AstNode* rhs) : BinaryNode(lhs, rhs) {}

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

class LessComparisonNode : public BinaryNode
{
public:
	LessComparisonNode() {}
	LessComparisonNode(AstNode* lhs, AstNode* rhs) : BinaryNode(lhs, rhs) {}

	virtual Result* Evaluate();
};

class NegationNode : public AstNode 
{
public:
	NegationNode() {}
	NegationNode(AstNode* operand) { AddChild(operand); }

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

class AssignmentNode : public BinaryNode
{
public:
	AssignmentNode() {}
	AssignmentNode(AstNode* identifierNode, AstNode* expression) : BinaryNode(identifierNode, expression) {}

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

class ArrayNode : public AstNode
{
public:
	ArrayNode() : arrayName("") {}
	ArrayNode(std::string name, AstNode* coords) : arrayName(name) { AddChild(coords); }
	virtual Result* Evaluate();
private:
	std::string arrayName;	
};


class NumExprList : public AstNode 
{
public:
	virtual Result* Evaluate() {}
};


#endif