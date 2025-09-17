/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnsila <abnsila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:48:20 by abnsila           #+#    #+#             */
/*   Updated: 2025/09/17 10:56:24 by abnsila          ###   ########.fr       */
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
	//! Restore last char (for token check in parsing)
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
	//! Stop at EOF or closed quote ""
	while (peek(stream) != EOF && peek(stream) != '\"')
	{
		c = getc(stream);
		if (c == '\\' && peek(stream) == '\"')
			c = getc(stream);
		//! Realloc (previous_size + new char + \0)
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
	//! Consume second "
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
	//! Empty Map
	if (accept(stream, '}'))
		return (1);
	//! Stop at EOF or closed {}
	while (peek(stream) != EOF && peek(stream) != '}')
	{
		//! Check for next pair only after first time ( the -> , come after (: )
		if (dst->map.size > 0)
			if (expect(stream, ',') == 0)
				return (-1);
		//! Begin of key string
		if (expect(stream, '\"') == 0)
			return (-1);
		//! " already consumed by expect(), start parsed
		key = parse_string(stream);
		if (key == NULL)
			return (-1);
		//! Time to value so we consume the -> :
		if (expect(stream, ':') == 0)
			return (-1);
		//! realloc for next pair the size is (previouse_size + 1)
		tmp = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		dst->map.data = tmp;
		dst->map.data[dst->map.size].key = key;
		//! Recursive the value by parsing -> json type (:
		if (parsing(&(dst->map.data[dst->map.size].value), stream) == -1)
		{
			free(key);
			return (-1);
		}
		dst->map.size++;
	}
	//! Consume second }
	if (expect(stream, '}'))
		return (1);
	return (-1);
}

int	parsing(json *dst, FILE *stream)
{
	int		c;
	char	*s;

	//! Uneceptable tokens at first
	c = peek(stream);
	if (c == EOF || c == '}' || c == ',' || c == ':')
	{
		unexpected(stream);
		return (-1);
	}
	//! Stop at EOF and end of Map because this parsing function is used as recursion for map (:
	while ((c = peek(stream)) != EOF && (c = peek(stream)) != '}')
	{
		//! String
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
		//! Map
		else if (c == '{')
		{
			// Consume first {
			getc(stream);
			if (parse_map(dst, stream) == -1)
				return (-1);
		}
		//! Integer
		else if (isdigit(c) || c == '-' || c == '+')
		{
			if (parse_integer(dst, stream) == -1)
				return (-1);
		}
		//! Next Map pair
		else if (c == ',')
			return (1);
		//! Unexpected tokens
		else
		{
			unexpected(stream);
			return (-1);
		}
	}
	//! Success parsing
	return (1);
}

int	argo(json *dst, FILE *stream)
{
	return(parsing(dst, stream));
}
