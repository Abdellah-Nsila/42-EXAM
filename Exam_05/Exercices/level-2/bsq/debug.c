#include "bsq.h"

void	print_headers(t_header* header)
{
	fprintf(stdout, "lines_num: %d\n", header->lines_num);
	fprintf(stdout, "empty_char: %c\n", header->empty_char);
	fprintf(stdout, "obstacle_char: %c\n", header->obstacle_char);
	fprintf(stdout, "full_char: %c\n", header->full_char);
}

void	print_bsq_cache_array(t_map* map)
{
	int row = 0;

	if (!map || !(map->cache_arr))
		return;
	fprintf(stdout, "===================== Cache Array map =====================\n");
	while (row < map->height)
	{
		int col = 0;
		while (col < map->width)
		{
			if (map->cache_arr[row])
				fprintf(stdout, "%d ", map->cache_arr[row][col]);
			col++;
		}
		fprintf(stdout, "\n\n");
		row++;
	}
}

void	print_bsq_map_array(t_map* map)
{
	int row = 0;

	if (!map || !(map->map_arr))
		return;
	fprintf(stdout, "===================== Output Array map =====================\n");
	while (row < map->height)
	{
		int col = 0;
		while (col < map->width)
		{
			if (map->map_arr[row])
				fprintf(stdout, "%c ", map->map_arr[row][col]);
			col++;
		}
		fprintf(stdout, "\n\n");
		row++;
	}
}