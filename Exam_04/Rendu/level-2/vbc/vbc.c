
# include "vbc.h"



// node	*get_expr_node(char *s, node *ret)
// {
// 	node	*n;

// 	if (!ret)
// 			return (NULL);
// 	n = NULL;
// 	n = calloc(1, sizeof(node));
// 	if (!n)
// 		return (NULL);
// 	if (*s == '(')
// 	{
// 		n = get_expr_node(*s + 1, ret);
// 		if (expect(&s, ')'))
// 			exit(EXIT_SUCCESS);
// 		if (!n)
// 			return (NULL);
// 		return (n);
// 	}
// 	else if (*s == '+')
// 	{
// 		n->type = ADD;
// 		n->l = get_expr_node(*s - 1, ret);
// 		if (!n->l)
// 			return (NULL);
// 		n->r = get_expr_node(*s + 1, ret);
// 		if (!n->r)
// 			return (free(n->l), NULL);
// 		return (n);
// 	}
// 	else if (isdigit(*s))
// 	{
// 		n->type = VAL;
// 		n->val = *s - '0';
// 		n->l = NULL;
// 		n->r = NULL;
// 		if (!n->l)
// 			return (NULL);
// 		return (n);
// 	}
// 	ret->l = get_expr_node(*s + 1, ret);
// }

node *parse_value(char **s)
{
	node	*n;

	if (**s == '(')
	{
		(*s)++;
		n = parse_add(s);
		if (expect(s, ')'))
			return (n);
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

node *parse_multi(char **s)
{
	node	*n;
	node	*right;
	node	*root;

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

node *parse_add(char **s)
{
	node	*n;
	node	*right;
	node	*root;

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
	if (*s) 
	{
		destroy_tree(ret);
		return (NULL);
	}
	return (ret);
}
