#include <AstNode.hpp>
#include <Scope.hpp>
#include <Structures.hpp>
#include <cassert>
#include <functional>

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

// TODO: Delete this function
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
	return BinaryNode::Apply(&Operable::operator+);
}

Result* SubtractionNode::Evaluate()
{
	return BinaryNode::Apply(&Operable::operator-);
}

Result* LiteralNode::Evaluate()
{	
	return datum;
}

Result * MultiplicationNode::Evaluate()
{	
	return BinaryNode::Apply(&Operable::operator*);	
}

Result * IdentifierNode::Evaluate()
{
	Result* res = Scope::GetCurrentScope().GetIdentifier(name);

	// use 'name' to get value from the variables of the current scope
	if(res == nullptr)
		return new ErrorResult("Identifier " + name + " is not declared");
	return res;
}

Result* ArrayNode::Evaluate()
{
	std::vector<AstNode*> _coords = children[0]->GetChildren();
	std::vector<Integer*> coords;
	coords.reserve(_coords.size());

	for(int i = 0; i < _coords.size(); i++)
	{
		Result* res = _coords[i]->Evaluate();
		Integer* int_res;

		if(TryConvert(res, int_res) == false || int_res <= 0)
			return new ErrorResult("Array index must be positive integer");

		coords.push_back(int_res);
	}

	std::cout << "Array indices: ";

	for(int i = 0; i < _coords.size(); i++)
		std::cout << *coords[i] << " ";

	std::cout << std::endl;

	Result* res = Scope::GetCurrentScope().GetIdentifier(arrayName);
	if(res == nullptr)
		return new ErrorResult("Identifier " + arrayName + " is not declared");
	return res;
}

Result * DivisionNode::Evaluate()
{
	return BinaryNode::Apply(&Operable::operator/);	
}

Result* LessComparisonNode::Evaluate()
{
	return BinaryNode::Apply(&Comparable::operator<);	
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

	// return BinaryNode::Apply<Result*, Result>(&Result::operator=);		This doesnt work since the function 'Apply' requires static ToString
}

Result* DeclarationNode::Evaluate()
{
	if(Scope::GetCurrentScope().AddIdentifier(identifierName, identifierObject) == false)
		return new ErrorResult("Identifier " + identifierName + " is already declared");

	// success
	return new Void();
}

Result* NegationNode::Evaluate()
{
	Result* operand = children[0]->Evaluate();
	Negatable* _operand;

	if(TryConvert(operand, _operand) == false)
		return new ErrorResult("Operand " + operand->MyTypeString() + " is not of " + Negatable::ToString() + " type");

	return _operand->operator~();
}