#ifndef BSQ_H
#define BSQ_H

#define _GNU_SOURCE
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"

#define TRUE 1
#define FALSE 0
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct	s_header
{
	int		lines_num;
	char	empty_char;
	char	obstacle_char;
	char	full_char;
}				t_header;

typedef struct	s_map
{
	t_header	header;
	int			width;
	int			height;
	char		**map_arr;
	int			**cache_arr;
}				t_map;

int		bsq(t_map* map);

//Parsing
int		parse_map(t_map* map, const char* file_path, int is_stdin);
void	free_bsq_arr(t_map* map);

// Debug
void	print_headers(t_header* header);
void	print_bsq_cache_array(t_map* map);
void	print_bsq_map_array(t_map* map);

#endif
