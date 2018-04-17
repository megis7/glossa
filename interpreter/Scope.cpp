#include "Scope.hpp"
#include <iostream>

// declare the static class variable
Scope Scope::currentScope;


bool Scope::AddIdentifier(std::string name, ValidResult * value)
{
	std::cout << "Adding identifier " << name << std::endl; 
	if (identifiers.find(name) != identifiers.end())	// already exists
		return false;

	identifiers.insert(make_pair(name, value));
	return true;
}

Result * Scope::GetIdentifier(std::string name) const
{
	auto iter = identifiers.find(name);
	if (iter == identifiers.end())		// TODO: Handle error more gracefully
		return nullptr;

	return iter->second;
}

Scope & Scope::GetCurrentScope()
{
	return currentScope;
}

void Scope::SetCurrentScope(Scope & scope)
{
	currentScope = scope;
}

void Scope::PrintAllVariables()
{
	for(auto iter = identifiers.begin(); iter != identifiers.end(); iter++)
		std::cout << iter->first << " = " << *iter->second << std::endl;
}