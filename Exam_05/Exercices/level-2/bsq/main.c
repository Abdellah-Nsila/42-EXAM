#include "bsq.h"

int main(int argc, char const *argv[])
{
	t_map	map;

	if (argc == 1)
	{
		// Read from stdin
		if (parse_map(&map, NULL, 1) != 0)
		{
			fprintf(stderr, "map error");
			return (1);
		}
	}
	else
	{
		// Read from file
		for (int i = 1; i < argc; i++)
		{
			if (parse_map(&map, argv[i], 1) != 0)
			{
				fprintf(stderr, "map error");
				return (1);
			}
		}
	}
	bsq(&map);
	free_bsq_arr(&map);
	return 0;
}
