#ifndef DATATYPES_H
#define DATATYPES_H

#include <types.h>
#include <Result.hpp>
#include <Operations.hpp>
#include <ostream>
#include <string>

class Integer : public ValidResult, public Operable, public Comparable, public Equitable, public Negatable
{
public:
	Integer() : datum(0) {}
	Integer(int value) : datum(value) {}

	virtual std::string ToString() const;

	virtual Type MyType() const { return INTEGER_TYPE; }
	virtual std::string MyTypeString() const { return "Integer"; }	

	virtual Result* operator= (Result * other);	

	virtual Result* operator +(const ValidResult* other);
	virtual Result* operator -(const ValidResult* other);
	virtual Result* operator *(const ValidResult* other);
	virtual Result* operator /(const ValidResult* other);

	virtual Result* operator <(const ValidResult* other);	
	virtual Result* operator >(const ValidResult* other);	
	virtual Result* operator <=(const ValidResult* other);	
	virtual Result* operator >=(const ValidResult* other);	

	virtual Result* operator ==(const ValidResult* other);
	virtual Result* operator !=(const ValidResult* other);

	virtual Result* operator ~() { return new Integer(-datum); }

private:
	int datum;
};

class Real : public ValidResult, public Operable, public Comparable, public Equitable, public Negatable
{
public:
	Real() : Real(0) {}
	Real(double value) : datum(value) {}

	virtual Type MyType() const { return REAL_TYPE; }

	virtual std::string ToString() const;
	virtual std::string MyTypeString() const { return "Real"; }

	virtual Result* operator= (Result * other);	

	virtual Result* operator +(const ValidResult* other);
	virtual Result* operator -(const ValidResult* other);	
	virtual Result* operator *(const ValidResult* other);
	virtual Result* operator /(const ValidResult* other);

	virtual Result* operator <(const ValidResult* other);	
	virtual Result* operator >(const ValidResult* other);	
	virtual Result* operator <=(const ValidResult* other);	
	virtual Result* operator >=(const ValidResult* other);

	virtual Result* operator ==(const ValidResult* other);
	virtual Result* operator !=(const ValidResult* other);

	virtual Result* operator ~() { return new Real(-datum); }
private:
	double datum;
	static double accuracy;
};

class Boolean : public ValidResult, public Equitable, public LComparable, public Negatable
{
public:
	Boolean() : Boolean(false) {}
	Boolean(bool value) : datum(value) {}

	virtual std::string ToString() const;
	virtual Type MyType() const { return BOOLEAN_TYPE; }	
	virtual std::string MyTypeString() const { return "Boolean"; }

	virtual Result* operator= (Result * other);

	bool IsTrue() const { return datum; }

	// LComparable implementation
	// These operations are short-circuited, but this happens after validating that "other" is of Boolean type
	virtual Result* operator &&(const ValidResult* other);
	virtual Result* operator ||(const ValidResult* other);

	virtual Result* operator ==(const ValidResult* other);
	virtual Result* operator !=(const ValidResult* other);

	virtual Result* operator ~();

	// Result* Boolean::operator !() const;


	// Define the two boolean values 'true' and 'false'
	static Boolean* BooleanTrue() { return new Boolean(true); }
	static Boolean* BooleanFalse() { return new Boolean(false); }

public:
	bool datum;
};

class String : public ValidResult, public Equitable, public Comparable
{
public:
	String() : String("") {}
	String(std::string value) : datum(value) {}

	virtual std::string ToString() const { return datum; }
	virtual Type MyType() const { return STRING_TYPE; }	
	virtual std::string MyTypeString() const { return "String"; }

	virtual Result* operator= (Result * other);

	virtual Result* operator <(const ValidResult* other);	
	virtual Result* operator >(const ValidResult* other);	
	virtual Result* operator <=(const ValidResult* other);	
	virtual Result* operator >=(const ValidResult* other);

	virtual Result* operator ==(const ValidResult* other);
	virtual Result* operator !=(const ValidResult* other);

private:
	std::string datum;
};

class Void : public ValidResult
{
public:
	virtual std::string ToString() const { return "void"; }
	virtual Type MyType() const { return VOID_TYPE; }	
	virtual std::string MyTypeString() const { return "Void"; }

	virtual Result* operator= (Result* other){ return new ErrorResult("Cannot assign to Void type"); }
};

#endif