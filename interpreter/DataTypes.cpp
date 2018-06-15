#include <DataTypes.hpp>
#include <string>
#include <cmath>

double Real::accuracy = 0.0001;

std::ostream & operator<<(std::ostream & str, Result const & data)
{
	str << data.ToString();
	return str;
}

Result * Integer::operator= (Result * other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");
	
	datum = _other->datum;
	return this;
}

Result * Integer::operator+(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	Integer* t = new Integer();
	t->datum = datum + _other->datum;
	return t;
}

Result * Integer::operator*(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	Integer* t = new Integer();
	t->datum = datum * _other->datum;

	return t;
}

Result * Integer::operator/(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");
	
	if(_other->datum == 0)
		return new ErrorResult("Cannot divide by 0");

	Real* t = new Real((double)datum / _other->datum);

	return t;
}

Result* Integer::operator -(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");
	
	Integer* t = new Integer(datum - _other->datum);

	return t;
}

Result* Integer::operator<(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum < _other->datum);
}

Result* Integer::operator<=(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum <= _other->datum);
}

Result* Integer::operator>(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum > _other->datum);
}

Result* Integer::operator>=(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum >= _other->datum);
}

Result* Integer::operator ==(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum == _other->datum);
}

Result* Integer::operator !=(const ValidResult* other)
{
	const Integer* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Integer");

	return new Boolean(datum != _other->datum);
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

Result* Real::operator= (Result * other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	datum = _other->datum;
	return this;
}

Result * Real::operator+(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	Real* r = new Real();
	r->datum = datum + _other->datum;

	return r;
}

Result* Real::operator -(const ValidResult * other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");
	
	Real* t = new Real(datum - _other->datum);

	return t;
}

Result * Real::operator*(const ValidResult * other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	Real* r = new Real();
	r->datum = datum * _other->datum;

	return r;
}

Result * Real::operator/(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	if(fabs(_other->datum) <= Real::accuracy)
		return new ErrorResult("Cannot divide by 0");

	Real* r = new Real();
	r->datum = datum / _other->datum;

	return r;
}

Result* Real::operator <(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	return new Boolean(datum < _other->datum);
}

Result* Real::operator <=(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	return new Boolean(datum <= _other->datum);
}

Result* Real::operator >(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	return new Boolean(datum > _other->datum);
}

Result* Real::operator >=(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	return new Boolean(datum >= _other->datum);
}

Result* Real::operator ==(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	return new Boolean(fabs(datum - _other->datum) <= accuracy);
}

Result* Real::operator !=(const ValidResult* other)
{
	const Real* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Real");

	return new Boolean(fabs(datum - _other->datum) > accuracy);
}

/********************************************************************************************/

std::string Boolean::ToString() const
{
	return (datum ? "true" : "false");
}

Result* Boolean::operator=(Result * other)
{
	Boolean* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	datum = _other->datum;
	return this;
}

Result* Boolean::operator &&(const ValidResult* other)
{
	const Boolean* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	return new Boolean(this->IsTrue() && _other->IsTrue());
}

Result* Boolean::operator ||(const ValidResult* other)
{
	const Boolean* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	return new Boolean(this->IsTrue() || _other->IsTrue());
}

Result* Boolean::operator ==(const ValidResult* other)
{
	const Boolean* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	return new Boolean(this->IsTrue() == _other->IsTrue());
}

Result* Boolean::operator !=(const ValidResult* other)
{
	const Boolean* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to Boolean");
	
	return new Boolean(this->IsTrue() != _other->IsTrue());
}

Result* Boolean::operator ~() 
{ 
	return new Boolean(!datum); 
}


// Result* Boolean::operator !() const
// {
// 	if(this->IsTrue())
// 		return &BooleanFalse;
	
// 	return &BooleanTrue;
// }

/********************************************************************************************/


Result* String::operator= (Result* other)
{
	String* _other;

	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to String");

	datum = _other->datum;
	return this;
}

Result* String::operator ==(const ValidResult* other)
{
	const String* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to String");
	
	return new Boolean(datum == _other->datum);
}

Result* String::operator !=(const ValidResult* other)
{
	const String* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to String");
	
	return new Boolean(datum != _other->datum);
}

Result* String::operator <(const ValidResult* other)
{
	const String* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to String");
	
	return new Boolean(datum < _other->datum);
}

Result* String::operator <=(const ValidResult* other)
{
	const String* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to String");
	
	return new Boolean(datum <= _other->datum);
}

Result* String::operator >(const ValidResult* other)
{
	const String* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to String");
	
	return new Boolean(datum > _other->datum);
}

Result* String::operator >=(const ValidResult* other)
{
	const String* _other;
	if(TryConvert(other, _other) == false)
		return new ErrorResult("Could not convert " + other->MyTypeString() + " object to String");
	
	return new Boolean(datum >= _other->datum);
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