#include "searchable_array_bag.hpp"

searchable_array_bag::searchable_array_bag(/* args */)
{
}

//TODO: do i need to contruct bag(copy) also ?
searchable_array_bag::searchable_array_bag(const searchable_array_bag& copy) : array_bag(copy), searchable_bag(copy)
{

}

searchable_array_bag&	searchable_array_bag::operator=(const searchable_array_bag& copy)
{
	if (this != &copy)
	{
		array_bag::operator=(copy);
	}
	return (*this);
}

searchable_array_bag::~searchable_array_bag()
{
}

bool	searchable_array_bag::has(int n) const
{
	for (int i = 0; i < this->size; i++)
	{
		if (data[i] == n)
			return (true);
	}
	return (false);
}
