#include "life.h"

void	free_life_arr(t_map* map)
{
	if (!map || !map->arr)
		return ;

	int	y = 0;
	while (y < map->heigth)
	{
		if (map->arr[y])
			free(map->arr[y]);
		y++;
	}
	free(map->arr);
}

void	print_life_arr(t_map* map)
{
	int	y = 0;
	int	x = 0;

	if (!map || !map->arr)
		return ;
	while (y < map->heigth)
	{
		x = 0;
		if (map->arr[y])
		{
			while (x < map->width)
			{
				putchar(map->arr[y][x]);
				x++;	
			}
		}
		putchar('\n');
		y++;
	}
}

int	is_valid_number(char* arg)
{
	int	num = 0;

	if (arg[0] == '0' && arg[1] == '\0')
		return (num);
	if ((num = atoi(arg)) > 0)
	{
		return (num);
	}
	printf("atoi error\n");
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

	map->arr = (char**)calloc(map->heigth, (sizeof(char*)));
	if (!map->arr)
		return (1);

	int	y = 0;
	int	x = 0;
	while (y < map->heigth)
	{
		x = 0;
		char*	row = (char*)calloc(map->width, sizeof(char));
		if (!row)
		{
			free_life_arr(map);
			return (1);
		}
		while (x < map->width)
		{
			row[x] = ' ';
			x++;	
		}
		map->arr[y] = row;
		y++;
	}


	y = 0;
	x = 0;
	int	draw = 0;
	while (1)
	{
		bytes = read(STDIN_FILENO, &c, 1);
		if (bytes == -1)
		{
			free_life_arr(map);
			return (1);
		}
		if (bytes == 0)
			break;

		if (c == 'x')
		{
			draw = !draw;
		}
		else if (c == 'w')
		{
			if (y > 0)
				y -= 1;
		}
		else if (c == 's')
		{
			if (y < (map->heigth - 1))
				y += 1;
		}
		else if (c == 'a')
		{
			if (x > 0)
				x -= 1;
		}
		else if (c == 'd')
		{
			if (x < (map->width - 1))
				x += 1;
		}
		if (draw)
			map->arr[y][x] = '0';
	}

	return (0);
}

char**	duplicate_map_arr(t_map* map)
{
	char**	copy = NULL;

	copy = (char**)calloc(map->heigth, (sizeof(char*)));
	if (!copy)
		return (NULL);

	int	y = 0;
	int	x = 0;
	while (y < map->heigth)
	{
		x = 0;
		char*	row = (char*)calloc(map->width, sizeof(char));
		if (!row && map->arr[y])
		{
			free_life_arr(map);
			return (NULL);
		}
		while (x < map->width)
		{
			row[x] = map->arr[y][x];
			x++;	
		}
		copy[y] = row;
		y++;
	}
	return(copy);
}

int	get_neighbors_count(t_map* map, int y, int x)
{
	int	start_y = (y - 1 < 0) ? 0 : y - 1;
	int	end_y = (y + 1 > (map->heigth - 1)) ? (map->heigth - 1) : y + 1;
	
	int	base_x = (x - 1 < 0) ? 0 : x - 1;
	int	end_x = (x + 1 > (map->width - 1)) ? (map->width - 1) : x + 1;

	int	count = 0;

	while (start_y <= end_y)
	{
		int start_x = base_x;
		while (start_x <= end_x)
		{
			if (!(start_y == y && start_x == x))
			{
				if (map->arr[start_y][start_x] == '0')
					count++;
			}
			start_x++;
		}
		start_y++;
	}
	return (count);
}

int	game_of_life(t_map* map, int iteration)
{
	if (iteration == 0)
		return (0);
	if (game_of_life(map, iteration - 1) != 0)
		return (1);

	char**	copy = NULL;
	copy = duplicate_map_arr(map);
	if (!copy)
		return (1);
	int	y = 0;
	int	x;
	int	neighbors_count;

	if (!map || !map->arr)
		return (1);
	while (y < map->heigth)
	{
		x = 0;
		if (map->arr[y])
		{
			neighbors_count = 0;
			while (x < map->width)
			{
				neighbors_count = get_neighbors_count(map, y, x);
				int is_alive = map->arr[y][x] == '0' ? 1 : 0;

				if (neighbors_count < 2 && is_alive) // die
				{
					copy[y][x] = ' ';
				}
				else if (neighbors_count < 4 && is_alive) // survive
				{
					// keep it
				}
				else if (neighbors_count > 3 && is_alive) // die
				{
					copy[y][x] = ' ';
				}
				else if (neighbors_count == 3 && !is_alive) // alive again
				{
					copy[y][x] = '0';
				}
				x++;	
			}
		}
		y++;
	}

	free_life_arr(map);
	map->arr = copy;
	return (0);
}
