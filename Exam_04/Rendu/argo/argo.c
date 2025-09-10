/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnsila <abnsila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:48:20 by abnsila           #+#    #+#             */
/*   Updated: 2025/09/10 17:56:33 by abnsila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "argo.h"

int	parsing(json *dst, FILE *stream);

int	peek(FILE *stream)
{
	int	c = getc(stream);
	ungetc(c, stream);
	return c;
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

int	accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		(void)getc(stream);
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

int	parse_integer(json *dst, FILE *stream)
{
	int		c;
	int		n;
	int		sign;

	n = 0;
	sign = 1;
	while (true)
	{
		c = getc(stream);
		while (c == '+' || c == '-')
		{
			if (c == '-')
				sign = -1;
			c = getc(stream);
		}
		if (isdigit(c))
			n = (n * 10) + (c - '0');
		else
			break ;
	}
	ungetc(c, stream);
	dst->type = INTEGER;
	dst->integer = n * sign;
	return (1);
}

char	*parse_string(FILE *stream)
{
	int		c;
	char	*s;
	char	*tmp;
	int		i = 0;

	s = (char *) calloc(1, sizeof(char));
	if (!s)
		return (NULL);
	s[0] = '\0';
	while (peek(stream) != EOF && peek(stream) != '\"')
	{
		c = getc(stream);
		if (c == '\\' && peek(stream) == '\"')
			c = getc(stream);
		tmp = realloc(s, (i + 2) * sizeof(char));
		if (!tmp)
		{
			free(s);
			return (NULL);
		}
		s = tmp;
		s[i++] = c;
		s[i] = '\0';
	}
	s[i] = '\0';
	// Consume second "
	if (expect(stream, '\"'))
	{	
		return (s);
	}
	return (NULL);
}

int	parse_map(json *dst, FILE *stream)
{
	char	*key;
	pair	*tmp;

	dst->type = MAP;
	dst->map.size = 0;
	dst->map.data = NULL;
	while (peek(stream) != EOF && peek(stream) != '}')
	{
		if (dst->map.size > 0)
			if (expect(stream, ',') == 0)
				return (-1);
		if (expect(stream, '\"') == 0)
			return (-1);
		// " already consumed by expect()
		key = parse_string(stream);
		if (key == NULL)
			return (-1);
		if (expect(stream, ':') == 0)
			return (-1);
		tmp = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		dst->map.data = tmp;
		dst->map.data[dst->map.size].key = key;
		if (parsing(&(dst->map.data[dst->map.size].value), stream) == -1)
			return (-1);
		dst->map.size++;
	}
	// Consume second }
	//TODO: I need to be sure about freeing this later
	if (expect(stream, '}'))
		return (1);
	return (-1);
}

int	parsing(json *dst, FILE *stream)
{
	int		c;
	char	*s;

	c = peek(stream);
	if (c == EOF || c == '}' || c == ',' || c == ':')
	{
		unexpected(stream);
		return (-1);
	}
	while ((c = peek(stream)) != EOF && (c = peek(stream)) != '}')
	{
		if (c == '\"')
		{
			// Consume first "
			getc(stream);
			s = parse_string(stream);
			if (s == NULL)
				return (-1);
			dst->type = STRING;
			dst->string = s;
		}
		else if (c == '{')
		{
			// Consume first {
			getc(stream);
			if (parse_map(dst, stream) == -1)
				return (-1);
		}
		else if (isdigit(c) || c == '-' || c == '+')
		{
			if (parse_integer(dst, stream) == -1)
				return (-1);
		}
		else if (c == ',')
			return (1);
		else
		{
			unexpected(stream);
			return (-1);
		}
	}
	return (1);
}

int	argo(json *dst, FILE *stream)
{
	// printf("exit code: %d\n", parsing(dst, stream));
	return(parsing(dst, stream));
}
