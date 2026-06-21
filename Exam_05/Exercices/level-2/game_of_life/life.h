#ifndef LIFE_H
#define LIFE_H

#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct	s_map
{
	int		width;
	int		heigth;
	int		iteration;
	char**	arr;
}				t_map;

void	free_life_arr(t_map* map);
void	print_life_arr(t_map* map);
int		generate_map(t_map* map, char* av[]);
int		game_of_life(t_map* map, int iteration);

#endif