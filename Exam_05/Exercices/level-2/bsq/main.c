#define _GNU_SOURCE
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "bsq.h"

int	parse_file(const char* path)
{	
	return (0);
}

int	bsq(const char* path)
{
	if (parse_file(path) != 0)
	{
		fprintf(stderr, "map error");
		return (1);
	}
	return (0);
}

int	parse_header(t_header* header, char* line)
{
	header->lines_num = 0;
	while (*line && (*line >= '0' && *line <= '9'))
	{
		header->lines_num = (header->lines_num * 10) + (*line - '0');
		line++;
	}
	if (*line && *line != ' ')	return (1);
	while (*line && *line == ' ')	line++;
	if (*line >= 32 && *line <= 126) //is printable empty_char
		header->empty_char = *line++;

	if (*line && *line != ' ')	return (1);
	while (*line && *line == ' ')	line++;
	if (*line >= 32 && *line <= 126) //is printable obstacle_char
		header->obstacle_char = *line++;

	if (*line && *line != ' ')	return (1);
	while (*line && *line == ' ')	line++;
	if (*line >= 32 && *line <= 126) //is printable full_char
		header->full_char = *line++;

	// return 0 => '\0' if parsing completed correctly, else something is wrong
	return (*line);
}

void	print_headers(t_header* header)
{
	fprintf(stdout, "lines_num: %d\n", header->lines_num);
	fprintf(stdout, "empty_char: %c\n", header->empty_char);
	fprintf(stdout, "obstacle_char: %c\n", header->obstacle_char);
	fprintf(stdout, "full_char: %c\n", header->full_char);
}

int	parse_lines(t_map* map, char* line)
{
	
	map->height += 1;
	return (0);
}

int	read_from_stdin()
{
	char*	line = NULL;
	size_t	size = 0;
	ssize_t	bytes_read;
	t_map	map;
	int		header_done = 0;
	int		current_line_len = 0;

	map.width = -1;
	map.height = 0;

	while ((bytes_read = getline(&line, &size, stdin)) != -1)
	{
		if (line[bytes_read - 1] == '\n')
		{
			line[bytes_read - 1] = '\0';
			current_line_len = bytes_read - 1;
		}
		else
			current_line_len = bytes_read;

		if (header_done)
		{
			fprintf(stdout, "current_line_len: %d\n", current_line_len);
			fprintf(stdout, "refrence_line_len: %d\n", map.width);
			if (map.width != -1)
			{
				if (current_line_len != map.width)
					return (1);
			}
			else
			{
				map.width = current_line_len;
			}
			if (parse_lines(&map, line) != 0)
				return (1);
			// print_map(&map);
		}
		else
		{
			if (parse_header(&(map.header), line) != 0)
				return (1);
			print_headers(&(map.header));
			header_done = 1;
		}
	}
	free(line);
	// bsq("input_file");
	return (0);
}

int main(int argc, char const *argv[])
{

	if (argc == 1)
	{
		if (read_from_stdin() != 0)
		{
			fprintf(stderr, "map error");
			return (1);
		}
		//TODO: else diplay the filled map
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			bsq(argv[i]);
		}
	}
	return 0;
}
