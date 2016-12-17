#pragma once

class AntiCheater
{
public:
	AntiCheater(void);
	~AntiCheater(void);

	AntiCheater( int value );
	inline void setValue( int value );

	int toInt() const;
	float toFloat() const;

	operator int();
	operator int() const;

	int operator+(int) const;
	int operator-(int) const;
	int operator*(int) const;
	int operator/(int) const;
	
	AntiCheater& operator=(int);
	AntiCheater& operator+=(int);
	AntiCheater& operator-=(int);
	AntiCheater& operator*=(int);
	AntiCheater& operator/=(int);

	AntiCheater& operator --();
	AntiCheater operator --(int);
	AntiCheater& operator ++();
	AntiCheater operator ++(int);

	int operator+( const AntiCheater& temp ) const;
	int operator-( const AntiCheater& temp ) const;
	int operator*( const AntiCheater& temp ) const;
	int operator/( const AntiCheater& temp ) const;
	
	AntiCheater& operator=( const AntiCheater& temp );
	AntiCheater& operator+=( const AntiCheater& temp );
	AntiCheater& operator-=( const AntiCheater& temp );
	AntiCheater& operator*=( const AntiCheater& temp );
	AntiCheater& operator/=( const AntiCheater& temp );	

private:
	int m_value;
	int m_rand;
};

