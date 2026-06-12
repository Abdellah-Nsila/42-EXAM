/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigint.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnsila <abnsila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 15:32:57 by abnsila           #+#    #+#             */
/*   Updated: 2026/06/12 20:26:39 by abnsila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bigint.hpp"

bigint::bigint()
{
	this->_digits = "0";
}

bigint::bigint(int num)
{
	std::stringstream	ss;
	while (true)
	{
		ss << ((num % 10));
		num /= 10;
		if (num == 0)
			break;
	}
	this->_digits = ss.str();
}

bigint::bigint(const bigint& copy)
{
	this->_digits = copy.getDigits();
}

bigint&	bigint::operator=(const bigint& copy)
{
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

const std::string	bigint::toStr() const
{
	std::string	str(this->_digits);
	std::reverse(str.begin(), str.end());
	return (str);
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
			sum += this->_digits[i] - 48;
		if (i < toAdd.getDigits().size())
			sum += toAdd.getDigits()[i] - 48;
		carry = sum / 10;
		if (i >= this->_digits.size())
			this->_digits.push_back((sum % 10) + 48);
		else
			this->_digits[i] = (sum % 10) + 48;
		i++;
	}
	if (carry)
	{
		this->_digits.push_back(carry + 48);
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

// 9234
// 1534

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

bool	bigint::operator>(const bigint& toComp) const
{
	return (!(*this < toComp));
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
	return (!(*this == toComp));
}

bool	bigint::operator<=(const bigint& toComp) const
{
	return ((*this < toComp) || (*this == toComp));
}

bool	bigint::operator>=(const bigint& toComp) const
{
	return ((*this > toComp) || (*this == toComp));
}



