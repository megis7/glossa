#include "AstNode.h"
#include "Scope.h"

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

	// Here we have a valid result token which defines the arithmetic and logical operators

	ValidResult* token_l = static_cast<ValidResult*>(operands.first);
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

	// Here we have a valid result token which defines the arithmetic and logical operators

	ValidResult* token_l = static_cast<ValidResult*>(operands.first);
	ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	Result* result = token_l->operator*(token_r);

	return result;
}

Result * IdentifierNode::Evaluate()
{
	// use 'name' to get value from the variables of the current scope
	return Scope::GetCurrentScope().GetIdentifier(name);
}

Result * DivisionNode::Evaluate()
{
	std::pair<Result*, Result*> operands = BinaryNode::GetBinaryOperands();

	if (operands.first->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.first));

	if(operands.second->IsSane() == false)
		return AstNode::HandleError(static_cast<ErrorResult*>(operands.second));

	// Here we have a valid result token which defines the arithmetic and logical operators

	ValidResult* token_l = static_cast<ValidResult*>(operands.first);
	ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	Result* result = token_l->operator/(token_r);

	return result;
}

Result* StatementsNode::Evaluate()
{	
	for(auto iter = children.begin(); iter != children.end(); iter++)
	{
		if(*iter == nullptr)			// TODO: FIX these errors (these are VOID results but we have not yet such a type)
			continue;

		Result* r = (*iter)->Evaluate();

		if(r == nullptr)		// TODO: FIX these errors
			continue;

		if(r->IsSane() == false)
			std::cout << "Error occured" << std::endl;
		
		std::cout << "Evaluated: " << *r << std::endl;
	}

	// TODO: return 'void' result
	return nullptr;
}	

Result* AssignmentNode::Evaluate()
{	
	std::pair<Result*, Result*> operands = BinaryNode::GetBinaryOperands();

	if (operands.first->IsSane() == false || operands.second->IsSane() == false)
	{
		// TOOD: Handle the error in a more graceful manner
		return nullptr;
	}

	*operands.first = operands.second;

	return operands.second;
}

Result* ConditionalStatements::Evaluate()
{
	Result* cond = condition->Evaluate();

	if(cond->IsSane() == false)		// Error
		return nullptr;

	Boolean* cond_bool = dynamic_cast<Boolean*>(cond);

	if(cond_bool->IsTrue())
	{
		statements->Evaluate();
		return &Boolean::BooleanTrue;
		// return special Result that shows 'cond_bool' was true (and no other else-if should be executed -- latch)
	}
	
	return &Boolean::BooleanFalse;
}

Result* ConditionalStructure::Evaluate()
{
	std::cout << "Executing " << children.size() << " conditionals" << std::endl;
	for(auto iter = children.begin(); iter != children.end(); iter++)
	{
		Result* res = (*iter)->Evaluate();
		if(((Boolean*)res)->IsTrue())
			break;
	}

	return nullptr;
}

Result* DeclarationNode::Evaluate()
{
	if(Scope::GetCurrentScope().AddIdentifier(identifierName, identifierObject) == false)
	{
		// return some error
	}

	// success
	return nullptr;
}

Result* IterativeStructure::Evaluate()
{
	assert(children.size() == 2);

	AstNode* condition = children[0];
	while(((Boolean*)condition->Evaluate())->IsTrue())
	{
		AstNode* statements = children[1];
		statements->Evaluate();
	}

	return nullptr;
}