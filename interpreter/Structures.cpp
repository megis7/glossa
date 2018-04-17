#include "Structures.hpp"
#include <cassert>

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

Result* SuccessionStructure::Evaluate()
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
