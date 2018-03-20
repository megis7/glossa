#ifndef RESULT_H
#define RESULT_H

#include <ostream>

// Abstract class that represents the result of a Node operation
class Result
{
public:
	virtual bool IsSane() = 0;
	virtual void Print(std::ostream& stream) const = 0;

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

	virtual Result* operator= (Result * other);
	virtual void Print(std::ostream& stream) const;		

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

	virtual Result* operator ==(ValidResult* other) { return nullptr; }
	virtual Result* operator !=(ValidResult* other) { return nullptr; }
	virtual Result* operator +(ValidResult const * other) = 0;
	virtual Result* operator -(ValidResult* other) { return nullptr; }
	virtual Result* operator *(ValidResult const* other) = 0;
	virtual Result* operator /(ValidResult const* other) = 0;
};

// A concrete realization of a valid result
class Integer : public ValidResult
{
public:
	Integer() : datum(0) {}
	Integer(int value) : datum(value) {}

	virtual void Print(std::ostream& stream) const;

	virtual Result* operator= (Result * other);	

	virtual Result* operator +(ValidResult const * other);
	virtual Result* operator *(ValidResult const * other);
	virtual Result* operator /(ValidResult const * other);

private:
	int datum;
};

class Real : public ValidResult
{
public:
	Real() : Real(0) {}
	Real(double value) : datum(value) {}

	virtual void Print(std::ostream& stream) const;

	virtual Result* operator= (Result * other);	

	virtual Result* operator +(ValidResult const * other);
	virtual Result* operator *(ValidResult const * other);
	virtual Result* operator /(ValidResult const * other);

private:
	double datum;

	static double accuracy;
};

class Boolean : public ValidResult
{
public:
	Boolean() : Boolean(false) {}
	Boolean(bool value) : datum(value) {}

	virtual void Print(std::ostream& stream) const;

	virtual Result* operator= (Result * other);

	virtual Result* operator +(ValidResult const * other) {return nullptr;}
	virtual Result* operator *(ValidResult const * other) {return nullptr;}
	virtual Result* operator /(ValidResult const * other) {return nullptr;}

	bool IsTrue() const { return datum; }

	// Define the two boolean values 'true' and 'false'
	static Boolean BooleanTrue;
	static Boolean BooleanFalse;

private:
	bool datum;
};

#endif