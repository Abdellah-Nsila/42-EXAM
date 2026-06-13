#include "vect2.hpp"

vect2::vect2(/* args */) : _x(0), _y(0)
{
}

vect2::vect2(int x, int y) : _x(x), _y(y)
{

}

vect2::vect2(const vect2& copy)
{
	*this = copy;
}

vect2&	vect2::operator=(const vect2& copy)
{
	if (this != &copy)
	{
		this->_x = copy.GetX();
		this->_y = copy.GetY();
	}
	return (*this);
}

vect2::~vect2()
{
}

int	vect2::operator[](int index) const
{
	if (index == 0)
		return (this->_x);
	if (index == 1)
		return (this->_y);
	return (0);
}

vect2&	vect2::operator++()
{
	this->_x += 1;
	this->_y += 1;
	return (*this);
}

vect2	vect2::operator++(int)
{
	vect2	obj(*this);
	++(*this);
	return (obj);
}
vect2&	vect2::operator--()
{
	this->_x -= 1;
	this->_y -= 1;
	return (*this);
}

vect2	vect2::operator--(int)
{
	vect2	obj(*this);
	--(*this);
	return (obj);
}

vect2&	vect2::operator+=(const vect2& toAdd)
{
	this->_x = this->_x + toAdd.GetX();
	this->_y = this->_y + toAdd.GetY();
	return (*this);
}

vect2&	vect2::operator-=(const vect2& toSub)
{
	this->_x = this->_x - toSub.GetX();
	this->_y = this->_y - toSub.GetY();
	return (*this);
}

vect2&	vect2::operator*=(const vect2& toMul)
{
	this->_x = this->_x * toMul.GetX();
	this->_y = this->_y * toMul.GetY();
	return (*this);
}

vect2	vect2::operator+(const vect2& toAdd) const
{
	vect2	v(*this);
	v += toAdd;
	return (v);
}

vect2	vect2::operator-(const vect2& toSub) const
{
	vect2	v(*this);
	v -= toSub;
	return (v);
}

vect2	vect2::operator*(const vect2& toMul) const
{
	vect2	v(*this);
	v *= toMul;
	return (v);
}

vect2	vect2::operator*=(int n)
{
	this->_x = this->_x * n;
	this->_y = this->_y * n;
	return (*this);
}

int	vect2::GetX() const
{
	return (this->_x);
}

int	vect2::GetY() const
{
	return (this->_y);
}

void	vect2::SetX(int x)
{
	this->_x = x;
}

void	vect2::SetY(int y)
{
	this->_y = y;
}

std::ostream&	operator<<(std::ostream& cout, const vect2& v)
{
	cout << "{" << v[0] << ", " << v[1] << "}";
	return (cout);
}



vect2			operator*(const vect2& v, int n)
{
	vect2	obj(v);
	obj.SetX(obj.GetX() * n);
	obj.SetY(obj.GetY() * n);
	return (obj);
}

vect2	operator*(int n, const vect2& v)
{
	return (v * n);
}
