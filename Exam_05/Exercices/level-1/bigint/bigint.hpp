/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bigint.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnsila <abnsila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 15:33:08 by abnsila           #+#    #+#             */
/*   Updated: 2026/06/12 20:23:42 by abnsila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

class bigint
{
	private:
		std::string	_digits;
	public:
		bigint();
		bigint(int num);
		bigint(const bigint& copy);
		bigint& operator=(const bigint& copy);
		~bigint();

		bigint&	operator+=(const bigint& toAdd);
		bigint	operator+(const bigint& toAdd) const;

		bigint&	operator++();
		bigint	operator++(int);

		bool	operator<(const bigint& toComp) const;
		bool	operator>(const bigint& toComp) const;
		bool	operator==(const bigint& toComp) const;
		bool	operator!=(const bigint& toComp) const;
		bool	operator<=(const bigint& toComp) const;
		bool	operator>=(const bigint& toComp) const;

		const std::string&	getDigits() const;
		const std::string	toStr() const;
};

std::ostream&	operator<<(std::ostream& cout, const bigint& other);
