#pragma once

#include "searchable_bag.hpp"

class set
{
	private:
		searchable_bag&	_bag;
	public:
		set(searchable_bag& bag);
		~set();

		void insert (int value);
		void insert (int *items, int count);
		void print() const;
		void clear();
		bool has(int value) const;

		const searchable_bag&	get_bag();

};
