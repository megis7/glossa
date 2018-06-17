#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cassert>

#include <Result.hpp>
#include <DataTypes.hpp>
#include <Scope.hpp>

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

template<typename P, typename iType>
class BinaryNode : public AstNode
{
public:
	BinaryNode(){}
	BinaryNode(AstNode* lhs, AstNode* rhs, Result* (iType::*_operation)(P)) : operation(_operation) { AddChild(lhs); AddChild(rhs); } 

	std::pair<Result*, Result*> GetBinaryOperands()
	{
		assert(children.size() == 2);
		assert(children[0] != nullptr && children[1] != nullptr);

		Result *lhs = children[0]->Evaluate(),
			*rhs = children[1]->Evaluate();

		return std::make_pair(lhs, rhs);
	}

	// Extracts binary operands from the 'children' array and applies the given operator by calling the 'BinaryOperation' function
	Result* Evaluate()
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

	Result* (iType::*operation)(P);
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

class AssignmentNode : public AstNode
{
public:
	AssignmentNode() {}
	AssignmentNode(AstNode* identifierNode, AstNode* expression) { AddChild(identifierNode); AddChild(expression); } //BinaryNode(identifierNode, expression) {}

	virtual Result* Evaluate();
};

class IdentifierList : public AstNode
{
public:
	IdentifierList() {}
	IdentifierList(std::string name) { AddIdentifier(name); }

	void AddIdentifier(std::string name) { identifierNames.push_back(name); }

	virtual Result* Evaluate() {}
public:
	std::vector<std::string> identifierNames;
};

template<class T>
class DeclarationNode : public AstNode
{
public:
	DeclarationNode() {}
	DeclarationNode(AstNode* identifiers) { AddChild(identifiers); }
	
	virtual Result* Evaluate()
	{
		IdentifierList* identifiers = dynamic_cast<IdentifierList*>(children[0]);

		for(std::string s : identifiers->identifierNames)
		{
			if(Scope::GetCurrentScope().AddIdentifier(s, new T()) == false)
				return new ErrorResult("Identifier " + s + " is already declared");
		}

		// success
		return new Void();
	}
private:
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