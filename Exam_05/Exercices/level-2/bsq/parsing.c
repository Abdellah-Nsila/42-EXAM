#include "bsq.h"

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


int	char_included_in_headers(t_header* header, char c)
{
	return (c == header->empty_char || c == header->obstacle_char);
}

void	free_bsq_arr(t_map* map)
{
	int row = 0;

	if (!map || !(map->arr))
		return;
	while (row < map->height)
	{
		if (map->arr[row])
				free(map->arr[row]);
		row++;
	}
	free(map->arr);
}

int	parse_lines(t_map* map, char* line)
{
	int	i = 0;

	if (map->height + 1 > map->header.lines_num)
		return (0);
	int *row = (int*)malloc(map->width * sizeof(int));
	if (!row)
		return (1);
	while (line[i])
	{
		if (line[i] == map->header.empty_char)
		{
			row[i] = 1;
		}
		else if (line[i] == map->header.obstacle_char)
		{
			row[i] = 0;
		}
		else
		{
			free(row);
			return (1);
		}
		i++;
	}
	map->arr[map->height] = row;
	map->height += 1;
	return (0);
}

int	read_from_file(t_map* map, FILE* file)
{
	char*	line = NULL;
	size_t	size = 0;
	ssize_t	bytes_read;
	int		header_done = 0;
	int		current_line_len = 0;

	map->width = -1;
	map->height = 0;
	map->arr = NULL;

	while ((bytes_read = getline(&line, &size, file)) != -1)
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
			if (map->width != -1)
			{
				if (current_line_len != map->width)
					return (1);
			}
			else
			{
				map->width = current_line_len;
			}
			if (parse_lines(map, line) != 0)
			{
				free(line);
				free_bsq_arr(map);
				return (1);
			}
		}
		else
		{
			if (parse_header(&(map->header), line) != 0)
				return (1);
			print_headers(&(map->header));
			header_done = 1;
			map->arr = (int**)malloc(map->header.lines_num * sizeof(int*));
			if (!map->arr)
				return (1);
		}
	}
	print_bsq_array(map);
	free(line);
	// bsq("input_file");
	return (0);
}

int	parse_map(t_map* map, const char* file_path, int is_stdin)
{
	int	status;

	if (is_stdin)
	{
		status = read_from_file(map, stdin);
	}
	else if (file_path)
	{
		FILE*	file = fopen(file_path, "r");
		if (file == NULL)
			status = 1;
		else
			status = read_from_file(map, file);
	}
	return (status);
}
