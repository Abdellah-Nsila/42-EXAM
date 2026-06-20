#include "life.h"

int	main(int ac, char* av[])
{
	t_map	map;
	map.arr = NULL;

	if (ac != 4)
	{
		// ./life width height iterations
		return (1);
	}
	if (generate_map(&map, av) != 0)
	{
		return (1);
	}
	game_of_life(&map);
	return (0);
}
