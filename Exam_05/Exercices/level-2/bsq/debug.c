#include "bsq.h"



void	print_headers(t_header* header)
{
	fprintf(stdout, "lines_num: %d\n", header->lines_num);
	fprintf(stdout, "empty_char: %c\n", header->empty_char);
	fprintf(stdout, "obstacle_char: %c\n", header->obstacle_char);
	fprintf(stdout, "full_char: %c\n", header->full_char);
}

void	print_bsq_array(t_map* map)
{
	int row = 0;

	if (!map || !(map->arr))
		return;
	fprintf(stdout, "===================== Array map =====================\n");
	while (row < map->height)
	{
		int col = 0;
		while (col < map->width)
		{
			if (map->arr[row])
				fprintf(stdout, "%d ", map->arr[row][col]);
			col++;
		}
		fprintf(stdout, "\n\n");
		row++;
	}
}

void	print_outputed_bsq_array(t_map* map)
{
	int row = 0;

	if (!map || !(map->arr))
		return;
	fprintf(stdout, "===================== Array map =====================\n");
	while (row < map->height)
	{
		int col = 0;
		while (col < map->width)
		{
			if (map->arr[row])
				fprintf(stdout, "%d ", map->arr[row][col]);
			col++;
		}
		fprintf(stdout, "\n\n");
		row++;
	}
}