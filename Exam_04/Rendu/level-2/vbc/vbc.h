#ifndef VBC_H
# define VBC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node *parse_value(char **s);
node *parse_multi(char **s);
node *parse_add(char **s);

node	*new_node(node n);
void	destroy_tree(node *n);
void	unexpected(char c);
int		accept(char **s, char c);
int		expect(char **s, char c);
int     eval_tree(node *tree);
node	*parse_expr(char *s);

#endif