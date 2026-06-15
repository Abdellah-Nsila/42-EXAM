#include "set.hpp"

set::set(searchable_bag& bag) : _bag(bag)
{

}

set::~set()
{
}

void	set::insert (int value)
{
	this->_bag.insert(value);
}

void	set::insert (int *items, int count)
{
	this->_bag.insert(items, count);
}

void	set::print() const
{
	this->_bag.print();
}

void	set::clear()
{
	this->_bag.clear();
}

bool	set::has(int value) const
{
	return (this->_bag.has(value));
}

const searchable_bag&	set::get_bag()
{
	return (this->_bag);
}
