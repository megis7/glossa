#include "Result.hpp"
#include <string>

double Real::accuracy = 0.0001;
Boolean Boolean::BooleanTrue = Boolean(true);
Boolean Boolean::BooleanFalse = Boolean(false);

std::ostream & operator<<(std::ostream & str, Result const & data)
{
	str << data.ToString();
	return str;
}

Result * Integer::operator+(ValidResult const * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	Integer* t = new Integer();
	t->datum = datum + _other->datum;
	return t;
}

Result * Integer::operator*(ValidResult const * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	Integer* t = new Integer();
	t->datum = datum * _other->datum;

	return t;
}

Result * Integer::operator/(ValidResult const * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");
	
	if(_other->datum == 0)
		return new ErrorResult("Cannot divide by 0");

	Real* t = new Real((double)datum / _other->datum);

	return t;
}

Result* Integer::operator -(ValidResult const * other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");
	
	Integer* t = new Integer(datum - _other->datum);

	return t;
}

Result * Integer::operator= (Result * other)
{
	const Integer* _other = dynamic_cast<Integer const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");
	
	datum = _other->datum;
	return this;
}

Result* Integer::operator<(ValidResult* other)
{
	Integer* _other = dynamic_cast<Integer *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum < _other->datum);
}


std::string Integer::ToString() const
{
	return std::to_string(datum);
}

/********************************************************************************************/

std::string Real::ToString() const
{
	return std::to_string(datum);
}

Result * Real::operator+(ValidResult const * other)
{
	const Real* _other = dynamic_cast<Real const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	Real* r = new Real();
	r->datum = datum + _other->datum;

	return r;
}

Result* Real::operator -(ValidResult const * other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");
	
	Real* t = new Real(datum - _other->datum);

	return t;
}

Result * Real::operator*(ValidResult const * other)
{
	const Real* _other = dynamic_cast<Real const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	Real* r = new Real();
	r->datum = datum * _other->datum;

	return r;
}

Result * Real::operator/(ValidResult const * other)
{
	const Real* _other = dynamic_cast<Real const *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

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
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	datum = _other->datum;
	return this;
}

Result* Real::operator <(ValidResult* other)
{
	Real* _other = dynamic_cast<Real *>(other);

	if(_other == nullptr)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum < _other->datum);
}	

/********************************************************************************************/

std::string Boolean::ToString() const
{
	return (datum ? "true" : "false");
}

Result * Boolean::operator=(Result * other)
{
	Boolean* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	datum = _other->datum;
	return this;
}

Result* Boolean::operator &&(ValidResult* other) const
{
	const Boolean* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	if(this->IsTrue() && _other->IsTrue())
		return &BooleanTrue;
	
	return &BooleanFalse;
}

Result* Boolean::operator ||(ValidResult* other) const
{
	const Boolean* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	if(this->IsTrue() || _other->IsTrue())
		return &BooleanTrue;
	
	return &BooleanFalse;
}

Result* Boolean::operator !() const
{
	if(this->IsTrue())
		return &BooleanFalse;
	
	return &BooleanTrue;
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

std::string ErrorResult::ToString() const
{
	return message;
}	