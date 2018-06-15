#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <Result.hpp>

class Comparable
{
public:
	virtual Result* operator <(const ValidResult* other) = 0;
	virtual Result* operator >(const ValidResult* other) = 0;
	virtual Result* operator <=(const ValidResult* other) = 0;
	virtual Result* operator >=(const ValidResult* other) = 0;

	static std::string ToString() { return "Comparable"; }
	
};

class Negatable 
{
public:
	virtual Result* operator ~() = 0;

	static std::string ToString() { return "Negatable"; }	
};

class LComparable
{
public:
	virtual Result* operator &&(const ValidResult* other) const = 0;
	virtual Result* operator ||(const ValidResult* other) const = 0;

	static std::string ToString() { return "LComparable"; }
	
};

class Equitable
{
public:
	virtual Result* operator !=(const ValidResult* other) = 0;
	virtual Result* operator ==(const ValidResult* other) = 0;

	static std::string ToString() { return "Equitable"; }
	
};

class Operable
{
public:
	virtual Result* operator +(const ValidResult* other) = 0;
	virtual Result* operator -(const ValidResult* other) = 0;
	virtual Result* operator *(const ValidResult* other) = 0;
	virtual Result* operator /(const ValidResult* other) = 0;

	static std::string ToString() { return "Operable"; }
};

#endif