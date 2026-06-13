#pragma once

#include "iostream"

class vect2
{
	private:
		int _x;
		int _y;
	public:
		vect2(/* args */);
		vect2(int x, int y);
		vect2(const vect2& copy);
		vect2& operator=(const vect2& copy);

		int	operator[](int index) const;

		vect2&	operator++();
		vect2	operator++(int);
		vect2&	operator--();
		vect2	operator--(int);

		vect2&	operator+=(const vect2& toAdd);
		vect2&	operator-=(const vect2& toSub);
		vect2&	operator*=(const vect2& toMul);

		vect2	operator+(const vect2& toAdd) const;
		vect2	operator-(const vect2& toSub) const;
		vect2	operator*(const vect2& toMul) const;

		vect2	operator*=(int n);



		int		GetX() const;
		int		GetY() const;
		void	SetX(int x);
		void	SetY(int y);

		~vect2();
};

vect2			operator*(const vect2& v, int n);
vect2			operator*(int n, const vect2& v);
std::ostream&	operator<<(std::ostream& cout, const vect2& v);
