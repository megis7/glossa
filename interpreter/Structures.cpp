#include <Structures.hpp>
#include <cassert>

Result* ConditionalStatements::Evaluate()
{
	Result* cond = condition->Evaluate();

	if(cond->IsSane() == false)		// Error when evaluating the condition
		return cond;				// return the cond which is of type ErrorResult and contains the actual error

	Boolean* cond_bool;

	if(TryConvert(cond, cond_bool) == false)
		return new ErrorResult("Could not convert ValidResult to Boolean for conditional");

	if(cond_bool->IsTrue())
	{
		statements->Evaluate();
		return Boolean::BooleanTrue(); 	// return special Result that shows 'cond_bool' was true (and no other else-if should be executed -- latch)
	}
	
	return Boolean::BooleanFalse();
}

Result* SuccessionStructure::Evaluate()
{	
	for(auto iter = children.begin(); iter != children.end(); iter++)
	{
		Result* r = (*iter)->Evaluate();

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
	assert(children.size() == 1);
	assert(children[0] != nullptr);

	AstNode* condition = children[0];
	AstNode* conditional_stmts = children[0];

	while(true)
	{
		Result* res = conditional_stmts->Evaluate();

		if(res->IsSane() == false)
			return res;

		Boolean* bool_res = Convert<Boolean>(res);
		
		if(bool_res->IsTrue() == false)
			break;
	}

	return new Void();
}
