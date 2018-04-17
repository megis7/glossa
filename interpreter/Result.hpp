#ifndef RESULT_H
#define RESULT_H

#include <ostream>

// Abstract class that represents the result of a Node operation
class Result
{
public:
	virtual bool IsSane() = 0;
	virtual std::string ToString() const = 0;

	virtual Result* operator= (Result * other) = 0;	
	
	friend std::ostream& operator<<(std::ostream& str, Result const& data);
};

// example requirement of the IsSane function. In the future we will expand this class
class ErrorResult : public Result
{
public:
	ErrorResult() : ErrorResult("") {}
	ErrorResult(std::string _message) : message(_message), handled(false) {}

	virtual bool IsSane() final { return false; }
	virtual std::string ToString() const;	

	virtual Result* operator= (Result * other);		

	bool IsHandled() { return handled; }
	void Handle() { handled = true; }
private:
	std::string message;
	bool handled;
};

// Abstract class that defines the operations of a valid Node result
class ValidResult : public Result
{
public:
	virtual bool IsSane() final { return true; }
};

/* Operations interfaces */

class Comparable
{
public:
	virtual Result* operator <(const ValidResult* other) = 0;
	virtual Result* operator <=(const ValidResult* other) {return nullptr;}
	virtual Result* operator >(const ValidResult* other) {return nullptr;}
	virtual Result* operator >=(const ValidResult* other) {return nullptr;}
};

class Equitable
{
public:
	virtual Result* operator !=(ValidResult* other) { return nullptr; }
	virtual Result* operator ==(ValidResult* other) { return nullptr; }
};

class Operable
{
public:
	virtual Result* operator +(ValidResult const * other) = 0;
	virtual Result* operator -(ValidResult* other) { return nullptr; }
	virtual Result* operator *(ValidResult const* other) = 0;
	virtual Result* operator /(ValidResult const* other) = 0;
};

/*--------------------------------------------------------------------------*/

// A concrete realization of a valid result
class Integer : public ValidResult, public Operable, public Comparable, public Equitable
{
public:
	Integer() : datum(0) {}
	Integer(int value) : datum(value) {}

	virtual std::string ToString() const;

	virtual Result* operator= (Result * other);	

	virtual Result* operator <(const ValidResult* other);	
	virtual Result* operator +(ValidResult const * other);
	virtual Result* operator *(ValidResult const * other);
	virtual Result* operator /(ValidResult const * other);

private:
	int datum;
};

class Real : public ValidResult, public Operable, public Comparable, public Equitable
{
public:
	Real() : Real(0) {}
	Real(double value) : datum(value) {}

	virtual std::string ToString() const;

	virtual Result* operator= (Result * other);	

	virtual Result* operator <(const ValidResult* other);	
	virtual Result* operator +(ValidResult const * other);
	virtual Result* operator *(ValidResult const * other);
	virtual Result* operator /(ValidResult const * other);

private:
	double datum;

	static double accuracy;
};

class Boolean : public ValidResult, public Equitable
{
public:
	Boolean() : Boolean(false) {}
	Boolean(bool value) : datum(value) {}

	virtual std::string ToString() const;

	virtual Result* operator= (Result * other);

	bool IsTrue() const { return datum; }

	// Define the two boolean values 'true' and 'false'
	static Boolean BooleanTrue;
	static Boolean BooleanFalse;

private:
	bool datum;
};

class Void : public ValidResult
{
public:
	virtual std::string ToString() const { return "void"; }

	virtual Result* operator= (Result * other){}
};

#endif