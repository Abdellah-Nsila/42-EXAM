#include "life.h"

int	is_valid_number(char* arg)
{
	int	num = 0;

	if (arg[0] == '0' && arg[1] == '\0')
		return (num);
	if ((num = atoi(arg)) > 0)
	{
		return (num);
	}
	return (-1);
}

int	parse_args(t_map* map, char* av[])
{
	int	num;

	if ((num = is_valid_number(av[1])) == -1)
		return (1);
	map->width = num;
	if ((num = is_valid_number(av[2])) == -1)
		return (1);
	map->heigth = num;
	if ((num = is_valid_number(av[3])) == -1)
		return (1);
	map->iteration = num;
	printf("width: %d, heigth: %d, iteration: %d\n", map->width, map->heigth, map->iteration);
	return (0);
}


int	generate_map(t_map* map, char* av[])
{
	char	c;
	int		bytes;

	if (parse_args(map, av) != 0)
	{
		return (1);
	}

	while (1)
	{
		bytes = read(STDIN_FILENO, &c, 1);
		if (bytes == -1)
			return (1);
		if (bytes == 0)
			break;
		putchar(c);
	}
	return (0);
}

int	game_of_life(t_map* map)
{
	return (0);
}
