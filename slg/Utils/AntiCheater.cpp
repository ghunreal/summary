#include "AntiCheater.h"
#include "Utils/UsefulFunc.h"

AntiCheater::AntiCheater(void)
{
	setValue(0);
}

AntiCheater::~AntiCheater(void)
{
}

AntiCheater::AntiCheater( int value )
{
	setValue(value);
}

void AntiCheater::setValue( int value )
{
	m_rand = UsefulFunc::createRand(1,10000,true);
	m_value = value - m_rand;
}

int AntiCheater::toInt() const
{
	return m_value + m_rand;
}

float AntiCheater::toFloat() const
{
	return (float)toInt();
}

AntiCheater::operator int ()
{
	return this->toInt();
}

AntiCheater::operator int() const
{
	return this->toInt();
}

AntiCheater& AntiCheater::operator+=(int temp)
{
	setValue(toInt() + temp);
	return *this;
}

AntiCheater& AntiCheater::operator-=(int temp)
{
	setValue(toInt() - temp);
	return *this;
}

AntiCheater& AntiCheater::operator*=(int temp)
{
	setValue(toInt() * temp);
	return *this;
}

AntiCheater& AntiCheater::operator/=(int temp)
{
	if ( temp != 0 )
	{
		setValue(toInt() / temp);
	}	
	return *this;
}

AntiCheater& AntiCheater::operator=(int temp)
{
	setValue(temp);
	return *this;
}

int AntiCheater::operator+(int temp) const
{
	return toInt()+temp;
}

int AntiCheater::operator-(int temp) const
{
	return toInt()-temp;
}

int AntiCheater::operator*(int temp) const
{
	return toInt()*temp;
}

int AntiCheater::operator/(int temp) const
{
	if( temp!= 0 )
	{
		return toInt()/temp;
	}
	return toInt();
}

AntiCheater& AntiCheater::operator+=( const AntiCheater& temp )
{
	setValue( this->toInt()+temp.toInt() );
	return *this;
}

AntiCheater& AntiCheater::operator-=( const AntiCheater& temp )
{
	setValue( this->toInt()-temp.toInt() );
	return *this;
}

AntiCheater& AntiCheater::operator*=( const AntiCheater& temp )
{
	setValue( this->toInt()*temp.toInt() );
	return *this;
}

AntiCheater& AntiCheater::operator/=( const AntiCheater& temp )
{
	if ( temp.toInt()!=0 )
	{
		setValue( this->toInt()/temp.toInt() );
	}	
	return *this;
}

AntiCheater& AntiCheater::operator --()
{
	setValue(toInt()-1);
	return *this;
}

AntiCheater AntiCheater::operator --(int)
{
	AntiCheater temp(toInt());
	setValue(toInt()-1);
	return temp;
}

AntiCheater& AntiCheater::operator ++()
{
	setValue(toInt()+1);
	return *this;
}

AntiCheater AntiCheater::operator ++(int)
{
	AntiCheater temp(toInt());
	setValue(toInt()+1);
	return temp;
}

AntiCheater& AntiCheater::operator=( const AntiCheater& temp )
{
	setValue( temp.toInt() );
	return *this;
}
