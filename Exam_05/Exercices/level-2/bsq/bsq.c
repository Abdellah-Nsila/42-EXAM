#include "bsq.h"

int	min_of_tree_num(int top, int diagonal, int left)
{
	return (MIN(MIN(top, diagonal), left));
}

int	bsq(t_map* map)
{
	// Create a cache matrix to evaluate indicies
	// A tracker variable for max last index square in the cache matrix
	int max_square_size = 0;
	int max_square_idx[2];
	// the square pkiapproach

	

	// if obstacle just keep it 0
	// if 1, we get the min(top, diagonal, left) indcies + 1
	int	row = 1;
	while (row < map->height)
	{
		int col = 1;
		while (col < map->width)
		{
			if (map->cache_arr[row][col] == 1)
			{
				map->cache_arr[row][col] = min_of_tree_num(
					map->cache_arr[row - 1][col],
					map->cache_arr[row - 1][col - 1],
					map->cache_arr[row][col - 1]
				) + 1;
				if (map->cache_arr[row][col] > max_square_size)
				{
					max_square_size = map->cache_arr[row][col];
					max_square_idx[0] = row;
					max_square_idx[1] = col;
				}
			}
			col++;
		}
		row++;
	}
	print_bsq_cache_array(map);


	int	start_row = max_square_idx[0];
	int	start_col = max_square_idx[1];

	while (start_row >= (max_square_idx[0] - (max_square_size - 1)))
	{
		start_col = max_square_idx[1];
		while (start_col >= (max_square_idx[1] - (max_square_size - 1)))
		{
			map->map_arr[start_row][start_col] = map->header.full_char;
			start_col--;
		}
		start_row--;
	}
	print_bsq_map_array(map);
	return (0);
}
