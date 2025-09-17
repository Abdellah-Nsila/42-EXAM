/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnsila <abnsila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:11:52 by abnsila           #+#    #+#             */
/*   Updated: 2025/09/17 10:01:48 by abnsila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

# define READ 0
# define WRITE 1

int	count_stages(char **cmds[])
{
	int	i;

	i = 0;
	while (cmds[i])
		i++;
	return (i);
}

int	setup_pipe(int (*pipefd)[2][2], int i, int has_next)
{
	if (has_next)
	{
		if (pipe((*pipefd)[i % 2]) != 0)
		{
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	redirect_pipe(int (*pipefd)[2][2], int i, int has_next)
{
	if (i > 0)
	{
		if (dup2((*pipefd)[(i + 1) % 2][READ], STDIN_FILENO) < 0)
			return (EXIT_FAILURE);
	}
	if (has_next)
	{
		if (dup2((*pipefd)[i % 2][WRITE], STDOUT_FILENO) < 0)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	close_pipes_in_child(int (*pipefd)[2][2], int i, int has_next)
{
	if (i > 0)
	{
		close((*pipefd)[(i + 1) % 2][READ]);
		close((*pipefd)[(i + 1) % 2][WRITE]);
	}
	if (has_next)
	{
		close((*pipefd)[i % 2][READ]);
		close((*pipefd)[i % 2][WRITE]);
	}
}

void	close_pipes_in_parent(int (*pipefd)[2][2], int i)
{
	if (i > 0)
	{	
		close((*pipefd)[(i + 1) % 2][READ]);
		close((*pipefd)[(i + 1) % 2][WRITE]);
	}
}

int	child_exec(char *cmd[], int (*pipefd)[2][2], int i, int has_next)
{
	if (redirect_pipe(pipefd, i, has_next) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	close_pipes_in_child(pipefd, i, has_next);
	execvp(cmd[0], cmd);
	exit(EXIT_FAILURE);
}

//TODO: -Wall -Wextra -Werror
int	picoshell(char **cmds[])
{
	int	i;
	int	count;
	int	has_next;
	int	pipefd[2][2];
	int	pid;
	int	status;

	if (!cmds || !*cmds)
		return (EXIT_FAILURE);
	i = 0;
	count = count_stages(cmds);
	while (i < count)
	{
		has_next = 0;
		if (i < count - 1)
			has_next = 1;
		if (setup_pipe(&pipefd, i, has_next) == EXIT_FAILURE)
		{
			close_pipes_in_parent(&pipefd, i);
			return (EXIT_FAILURE);
		}
		pid = fork();
		if (pid < 0)
		{
			close_pipes_in_parent(&pipefd, i);
			return (EXIT_FAILURE);
		}
		// Child
		if (pid == 0)
		{
			if (child_exec(cmds[i], &pipefd, i, has_next) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		// Parent
		close_pipes_in_parent(&pipefd, i);
		i++;
	}

	while (wait(&status) > 0)
	{
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


// #include <stdio.h>
// int	main(int argc, char **argv)
// {
// 	int	cmds_size = 1;
// 	for (int i = 1; i < argc; i++)
// 	{
// 		if (!strcmp(argv[i], "|"))
// 			cmds_size++;
// 	}
// 	char ***cmds = calloc(cmds_size + 1, sizeof(char **));
// 	if (!cmds)
// 	{
// 		dprintf(2, "Malloc error: %m\n");
// 		return 1;
// 	}
// 	cmds[cmds_size] = NULL; //need confirmation for this line
// 	cmds[0] = argv + 1;
// 	int cmds_i = 1;
// 	for (int i = 1; i < argc; i++)
// 	{
// 		if (!strcmp(argv[i], "|"))
// 		{
// 			cmds[cmds_i] = argv + i + 1;
// 			argv[i] = NULL;
// 			cmds_i++;
// 		}
// 	}
// 	int ret = picoshell(cmds);
// 	if (ret)
// 		perror("picoshell");
// 	free(cmds);
// 	return ret;
// }
