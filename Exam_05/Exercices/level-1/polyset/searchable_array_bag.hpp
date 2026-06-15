#pragma once

#include "array_bag.hpp"
#include "searchable_bag.hpp"

class searchable_array_bag : public array_bag, public searchable_bag
{
	private:
		/* data */
	public:
		searchable_array_bag(/* args */);
		searchable_array_bag(const searchable_array_bag& copy);
		searchable_array_bag&	operator=(const searchable_array_bag& copy);
		~searchable_array_bag();

		bool	has(int n) const;

};

