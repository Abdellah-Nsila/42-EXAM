#include "bsq.h"

// if you run the program you gonna see some debug logs [for testing purpose of course]

int main(int argc, char const *argv[])
{
	t_map	map;

	map.width = -1;
	map.height = 0;
	map.map_arr = NULL;
	map.cache_arr = NULL;

	if (argc == 1)
	{
		// Read from stdin
		if (parse_map(&map, NULL, TRUE) != 0)
		{
			fprintf(stderr, "map error\n");
			return (1);
		}
	}
	else
	{
		// Read from file
		for (int i = 1; i < argc; i++)
		{
			if (parse_map(&map, argv[i], FALSE) != 0)
			{
				fprintf(stderr, "map error\n");
				return (1);
			}
		}
	}
	bsq(&map);
	free_bsq_arr(&map);
	return 0;
}
