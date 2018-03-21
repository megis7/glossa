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

	// ValidResult* token_l = static_cast<ValidResult*>(operands.first);
	// ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	OperableResult* token_l = dynamic_cast<OperableResult*>(operands.first);
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

	// ValidResult* token_l = static_cast<ValidResult*>(operands.first);
	// ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	OperableResult* token_l = dynamic_cast<OperableResult*>(operands.first);
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

	// Here we have a valid result token which defines the arithmetic and logical operators

	// ValidResult* token_l = static_cast<ValidResult*>(operands.first);
	// ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	OperableResult* token_l = dynamic_cast<OperableResult*>(operands.first);
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

	// ValidResult* token_l = static_cast<ValidResult*>(operands.first);
	// ValidResult* token_r = static_cast<ValidResult*>(operands.second);

	ComparableResult* token_l = dynamic_cast<ComparableResult*>(operands.first);
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

Result* StatementsNode::Evaluate()
{	
	for(auto iter = children.begin(); iter != children.end(); iter++)
	{
		if(*iter == nullptr)			// TODO: FIX these errors (these are VOID results but we have not yet such a type)
		{
			std::cout << "statements found nullptr" << std::endl;
			continue;
		}

		Result* r = (*iter)->Evaluate();

		if(r == nullptr)		// TODO: FIX these errors
		{
			std::cout << "statements found nullptr result" << std::endl;
			continue;
		}

		if(r->IsSane() == false)
		{
			// return r;			Cease execution
			std::cout << "Error occured: " << *r << std::endl;
			continue;
		}
		
		std::cout << "Evaluated: " << *r << std::endl;
	}

	return new Void();
}	

Result* ConditionalStatements::Evaluate()
{
	Result* cond = condition->Evaluate();

	if(cond->IsSane() == false)		// Error when evaluating the condition
		return cond;				// return the cond which contains the actual error

	Boolean* cond_bool = dynamic_cast<Boolean*>(cond);

	if(cond_bool == nullptr)		// returned condition was of a non-boolean type
		return new ErrorResult("Could not convert ValidResult to Boolean for conditional");

	if(cond_bool->IsTrue())
	{
		statements->Evaluate();
		return &Boolean::BooleanTrue; 	// return special Result that shows 'cond_bool' was true (and no other else-if should be executed -- latch)
	}
	
	return &Boolean::BooleanFalse;
}

Result* ConditionalStructure::Evaluate()
{
	std::cout << "Executing " << children.size() << " conditionals" << std::endl;
	for(auto iter = children.begin(); iter != children.end(); iter++)
	{
		Result* res = (*iter)->Evaluate();
		if(((Boolean*)res)->IsTrue())			// if the conditional statements returned true, then we should not execute any other branch of the conditional structure 
			break;
	}

	return new Void();
}

Result* IterativeStructure::Evaluate()
{
	assert(children.size() == 2);
	assert(children[0] != nullptr && children[1] != nullptr);

	AstNode* condition = children[0];
	Result* res = condition->Evaluate();

	if(res->IsSane() == false)
		return res;

	Boolean* bool_res = dynamic_cast<Boolean*>(res);
	if(bool_res == nullptr)
		return new ErrorResult("Could not convert ValidResult to Boolean for while loop condition");

	while(bool_res->IsTrue())
	{
		AstNode* statements = children[1];
		statements->Evaluate();

		bool_res = dynamic_cast<Boolean*>(condition->Evaluate());		// safely(?) re-evaluate the boolean condition 
	}

	return new Void();
}