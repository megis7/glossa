#include "AstNode.hpp"
#include "Scope.hpp"

#include <cassert>

void AstNode::AddChild(AstNode* child)
{
	children.push_back(child);
}

void AstNode::AddChildFront(AstNode* child)
{
	children.insert(children.begin(), child);
}

bool AstNode::IsLeaf()
{
	return children.size() == 0;
}

Result* AstNode::HandleError(ErrorResult* error)
{
	if(error->IsHandled() == false)
	{
		std::cerr << *error;
		error->Handle();
	}

	return error;
}

AstNode::~AstNode()
{
	for (auto iter = children.begin(); iter != children.end(); iter++)
		delete *iter;
}

std::pair<Result*, Result*> BinaryNode::GetBinaryOperands()
{
	assert(children.size() == 2);
	assert(children[0] != nullptr && children[1] != nullptr);

	Result *lhs = children[0]->Evaluate(),
		   *rhs = children[1]->Evaluate();

	return std::make_pair(lhs, rhs);
}

Result* AdditionNode::Evaluate()
{	
	std::pair<Result*, Result*> operands = BinaryNode::GetBinaryOperands();

	if (operands.first->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.first));

	if(operands.second->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.second));

	Operable* token_l = dynamic_cast<Operable*>(operands.first);
	ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	Result* result = token_l->operator+(token_r);

	return result;
}

Result* LiteralNode::Evaluate()
{	
	return datum;
}

Result * MultiplicationNode::Evaluate()
{	
	std::pair<Result*, Result*> operands = BinaryNode::GetBinaryOperands();

	if (operands.first->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.first));

	if(operands.second->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.second));

	Operable* token_l = dynamic_cast<Operable*>(operands.first);
	ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	Result* result = token_l->operator*(token_r);

	return result;
}

Result * IdentifierNode::Evaluate()
{
	// use 'name' to get value from the variables of the current scope
	return Scope::GetCurrentScope().GetIdentifier(name);		// TODO: This can return nullptr when the variable is not declared in the scope
}

Result * DivisionNode::Evaluate()
{
	std::pair<Result*, Result*> operands = BinaryNode::GetBinaryOperands();

	if (operands.first->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.first));

	if(operands.second->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.second));

	Operable* token_l = dynamic_cast<Operable*>(operands.first);
	ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	Result* result = token_l->operator/(token_r);

	return result;
}

Result* LessComparisonNode::Evaluate()
{
	std::pair<Result*, Result*> operands = BinaryNode::GetBinaryOperands();

	if (operands.first->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.first));

	if(operands.second->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.second));

	Comparable* token_l = dynamic_cast<Comparable*>(operands.first);
	ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	return token_l->operator<(token_r);
}

Result* AssignmentNode::Evaluate()
{	
	std::pair<Result*, Result*> operands = BinaryNode::GetBinaryOperands();

	if (operands.first->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.first));

	if(operands.second->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.second));

	operands.first->operator=(operands.second);

	return operands.second;			// TODO: Perhaps the return type of an assignment will be void 
}

Result* DeclarationNode::Evaluate()
{
	if(Scope::GetCurrentScope().AddIdentifier(identifierName, identifierObject) == false)
		return new ErrorResult("Identifier " + identifierName + " is already declared");

	// success
	return new Void();
}