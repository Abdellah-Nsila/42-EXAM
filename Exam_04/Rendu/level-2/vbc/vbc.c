
# include "vbc.h"

node    *new_node(node n)
{
	node *ret = calloc(1, sizeof(n));
	if (!ret)
		return (NULL);
	*ret = n;
	return (ret);
}

void    destroy_tree(node *n)
{
	if (!n)
		return ;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}

void    unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of file\n");
}

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int expect(char **s, char c)
{
	if (accept(s, c))
		return (1);
	unexpected(**s);
	return (0);
}

int eval_tree(node *tree)
{
	switch (tree->type)
	{
		case ADD:
			return (eval_tree(tree->l) + eval_tree(tree->r));
		case MULTI:
			return (eval_tree(tree->l) * eval_tree(tree->r));
		case VAL:
			return (tree->val);
	}
	return (0);
}

node	*parse_value(char **s)
{
	node	*n;

	if (**s == '(')
	{
		(*s)++;
		n = parse_add(s);
		if (expect(s, ')'))
			return (n);
		destroy_tree(n);
		return (NULL);
	}
	else if (isdigit(**s))
	{
		n = calloc(1, sizeof(node));
		n->type = VAL;
		n->val = **s - '0';
		n->l = NULL;
		n->r = NULL;
		(*s)++;
		return (n);
	}
	return (NULL);
}

node	*parse_multi(char **s)
{
	node	*root;
	node	*n;
	node	*right;

	n = parse_value(s);
	while (**s == '*')
	{
		(*s)++;
		right = parse_value(s);
		root = calloc(1, sizeof(node));
		root->type = MULTI;
		root->l = n;
		root->r = right;
		n = root;
	}
	return (n);
}

node	*parse_add(char **s)
{
	node	*root;
	node	*n;
	node	*right;

	n = parse_multi(s);
	while (**s == '+')
	{
		(*s)++;
		right = parse_multi(s);
		root = calloc(1, sizeof(node));
		root->type = ADD;
		root->l = n;
		root->r = right;
		n = root;
	}
	return (n);
}

node	*parse_expr(char *s)
{
	node *ret;

	ret = parse_add(&s);
	if (!ret)
		return (NULL);
	expect(&s, EOF);
	if (*s) 
	{
		destroy_tree(ret);
		return (NULL);
	}
	return (ret);
}
