/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigint.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnsila <abnsila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 15:32:57 by abnsila           #+#    #+#             */
/*   Updated: 2026/06/13 18:13:38 by abnsila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bigint.hpp"

bigint::bigint()
{
	this->_digits = "0";
}

bigint::bigint(int num)
{
	if (num < 0)
		num = 0;
	std::stringstream	ss;
	ss << num;
	this->_digits = ss.str();
	std::reverse(this->_digits.begin(),this->_digits.end());
}

bigint::bigint(const bigint& copy)
{
	this->_digits = copy.getDigits();
}

bigint&	bigint::operator=(const bigint& copy)
{
	if (this != &copy)
		this->_digits = copy.getDigits();
	return (*this);
}

bigint::~bigint()
{
}

const std::string&	bigint::getDigits() const
{
	return (this->_digits);
}

std::string	bigint::toStr() const
{
	std::string	str(this->_digits);
	std::reverse(str.begin(), str.end());
	return (str);
}

unsigned int	stringToUint(const std::string& str)
{
	std::stringstream	ss(str);
	unsigned int n = 0;
	ss >> n;
	return (n);
}

std::ostream&	operator<<(std::ostream& cout, const bigint& other)
{
	std::string	reversedDigits(other.getDigits());
	std::reverse(reversedDigits.begin(), reversedDigits.end());
	cout << reversedDigits;
	return (cout);
}

bigint&	bigint::operator+=(const bigint& toAdd)
{
	std::size_t	i = 0;
	int 		carry = 0;
	
	while (i < this->_digits.size() || i < toAdd.getDigits().size())
	{
		int	sum = carry;

		if (i < this->_digits.size())
			sum += this->_digits[i] - '0';
		if (i < toAdd.getDigits().size())
			sum += toAdd.getDigits()[i] - '0';
		carry = sum / 10;
		if (i >= this->_digits.size())
			this->_digits.push_back((sum % 10) + '0');
		else
			this->_digits[i] = (sum % 10) + '0';
		i++;
	}
	if (carry)
	{
		this->_digits.push_back(carry + '0');
	}
	return (*this);
}

bigint	bigint::operator+(const bigint& toAdd) const
{
	bigint	obj(*this);
	
	obj += toAdd;
	return (obj);
}

bigint&	bigint::operator++()
{
	*this += bigint(1);
	return (*this);
}

bigint	bigint::operator++(int)
{
	bigint	obj(*this);
	*this += bigint(1);
	return (obj);
}

bigint&	bigint::operator<<=(unsigned int n)
{
	this->_digits.insert(0, n, '0');
	return (*this);
}

bigint	bigint::operator<<(unsigned int n) const
{
	bigint	obj(*this);
	obj <<= n;
	return (obj);
}

bigint&	bigint::operator>>=(unsigned int n)
{
	// 210000000042 >> 2 = 2100000000
	if (n >= this->_digits.length())
		this->_digits = "0";
	else
	{
		this->_digits.erase(0, n);
		if (this->_digits.empty())
			this->_digits = "0";
	}
	return (*this);
}

bigint	bigint::operator>>(unsigned int n) const
{
	bigint	obj(*this);
	obj >>= n;
	return (obj);
}

bigint&	bigint::operator<<=(const bigint& toShift)
{
	return (*this <<= stringToUint(toShift.toStr()));
}

bigint	bigint::operator<<(const bigint& toShift) const
{
	bigint	obj(*this);
	obj <<= toShift;
	return (obj);
}

bigint&	bigint::operator>>=(const bigint& toShift)
{
	// 210000000042 >> 2 = 2100000000	
	return (*this >>= stringToUint(toShift.toStr()));
}

bigint	bigint::operator>>(const bigint& toShift) const
{
	bigint	obj(*this);
	obj >>= toShift;
	return (obj);
}

bool	bigint::operator<(const bigint& toComp) const
{
	std::string	s1 = this->toStr();
	std::string	s2 = toComp.toStr();
	std::size_t	len1 = s1.length();
	std::size_t	len2 = s2.length();
	if (len1 != len2)
		return (len1 < len2);
	return (s1 < s2);
}

// I duplicate code cuz of this case 42 < 42 => false (!false) = true => 42 > 42 = true is wrong
bool	bigint::operator>(const bigint& toComp) const
{
	std::string	s1 = this->toStr();
	std::string	s2 = toComp.toStr();
	std::size_t	len1 = s1.length();
	std::size_t	len2 = s2.length();
	if (len1 != len2)
		return (len1 > len2);
	return (s1 > s2);
}

bool	bigint::operator==(const bigint& toComp) const
{
	std::string	s1 = this->toStr();
	std::string	s2 = toComp.toStr();
	std::size_t	len1 = s1.length();
	std::size_t	len2 = s2.length();
	if (len1 != len2)
		return (false);
	return (s1 == s2);
}

bool	bigint::operator!=(const bigint& toComp) const
{
	std::string	s1 = this->toStr();
	std::string	s2 = toComp.toStr();
	std::size_t	len1 = s1.length();
	std::size_t	len2 = s2.length();
	if (len1 != len2)
		return (true);
	return (s1 != s2);
}

bool	bigint::operator<=(const bigint& toComp) const
{
	return ((*this < toComp) || (*this == toComp));
}

bool	bigint::operator>=(const bigint& toComp) const
{
	return ((*this > toComp) || (*this == toComp));
}
