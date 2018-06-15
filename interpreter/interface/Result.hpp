#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <ostream>

#include <types.h>

class Presentable
{
public:
	virtual std::string ToString() const = 0;
};

// Abstract class that represents the result of a Node operation
class Result : public Presentable
{
public:
	virtual bool IsSane() = 0;

	virtual Type MyType() const = 0;
	virtual std::string MyTypeString() const = 0;

	virtual Result* operator= (Result * other) = 0;	
	
	friend std::ostream& operator<<(std::ostream& str, Result const& data);
};

// Abstract class that defines the operations of a valid Node result
class ValidResult : public Result
{
public:
	virtual bool IsSane() final { return true; }
};

// example requirement of the IsSane function. In the future we will expand this class
class ErrorResult : public Result
{
public:
	ErrorResult() : ErrorResult("") {}
	ErrorResult(std::string _message) : message(_message), handled(false) {}

	virtual bool IsSane() final { return false; }
	virtual std::string ToString() const;	

	virtual Type MyType() const { return ERROR_TYPE; }
	virtual std::string MyTypeString() const { return "Error"; }

	virtual Result* operator= (Result * other);		

	bool IsHandled() { return handled; }
	void Handle() { handled = true; }
private:
	std::string message;
	bool handled;
};

template <class From, class To>
bool TryConvert(From* x, To* &y)
{
	y = dynamic_cast<To*>(x);
	return y != nullptr; 
}

template <class T>
T* Convert(Result* x){ return dynamic_cast<T*>(x); }

#endif