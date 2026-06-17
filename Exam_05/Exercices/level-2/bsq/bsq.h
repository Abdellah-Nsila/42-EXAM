#ifndef BSQ_H
#define BSQ_H

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
	char		**map;
}				t_map;

#endif
