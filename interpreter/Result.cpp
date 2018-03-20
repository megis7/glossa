#include "Result.h"
#include <typeinfo>

double Real::accuracy = 0.0001;
Boolean Boolean::BooleanTrue = Boolean(true);
Boolean Boolean::BooleanFalse = Boolean(false);

std::ostream & operator<<(std::ostream & str, Result const & data)
{
	data.Print(str);
	return str;
}

Result * Integer::operator+(ValidResult const * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Integer");

	Integer* t = new Integer();
	t->datum = datum + _other->datum;
	return t;
}

Result * Integer::operator*(ValidResult const * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Integer");

	Integer* t = new Integer();
	t->datum = datum * _other->datum;

	return t;
}

Result * Integer::operator/(ValidResult const * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Integer");
	
	if(_other->datum == 0)
		return new ErrorResult("Cannot divide by 0");

	Real* t = new Real((double)datum / _other->datum);

	return t;
}

Result * Integer::operator= (Result * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Integer");
	
	datum = _other->datum;
	return this;
}

void Integer::Print(std::ostream & stream) const
{
	stream << datum;
}

/********************************************************************************************/

void Real::Print(std::ostream & stream) const
{
	stream << datum;
}

Result * Real::operator+(ValidResult const * other)
{
	const Real* _other = dynamic_cast<Real const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Real");

	Real* r = new Real();
	r->datum = datum + _other->datum;

	return r;
}

Result * Real::operator*(ValidResult const * other)
{
	const Real* _other = dynamic_cast<Real const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Real");

	Real* r = new Real();
	r->datum = datum * _other->datum;

	return r;
}

Result * Real::operator/(ValidResult const * other)
{
	const Real* _other = dynamic_cast<Real const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Real");

	if(abs(_other->datum) <= Real::accuracy)
		return new ErrorResult("Cannot divide by 0");

	Real* r = new Real();
	r->datum = datum / _other->datum;

	return r;
}

Result* Real::operator= (Result * other)
{
	const Real* _other = dynamic_cast<Real const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Real");

	datum = _other->datum;
	return this;
}

/********************************************************************************************/

void Boolean::Print(std::ostream& stream) const
{
	stream << (datum ? "true" : "false");
}

Result * Boolean::operator=(Result * other)
{
	const Boolean* _other = dynamic_cast<Boolean const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert ValidResult object to Boolean");
	
	datum = _other->datum;
	return this;
}

/********************************************************************************************/

Result* ErrorResult::operator= (Result * other)
{
	if(other->IsSane())
		return new ErrorResult("Cannot assign sane result to ErrorResult");

	ErrorResult * err = dynamic_cast<ErrorResult *>(other);
	message = err->message;
	return this;
}

void ErrorResult::Print(std::ostream& stream) const
{
	stream << message << std::endl;
}	