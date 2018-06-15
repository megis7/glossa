#ifndef SCOPE_H
#define SCOPE_H

#include <map>
#include <string>

#include <Result.hpp>

// Defines a variable collection that is grouped into the scope
class Scope
{
public:
	bool AddIdentifier(std::string name, ValidResult* value);
	Result* GetIdentifier(std::string name) const;

	static Scope & GetCurrentScope();
	static void SetCurrentScope(Scope& scope);

	void PrintAllVariables();

private:
	std::map<std::string, ValidResult*> identifiers;

	static Scope currentScope;
};

#endif