/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnsila <abnsila@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:34:52 by abnsila           #+#    #+#             */
/*   Updated: 2025/09/08 18:46:56 by abnsila          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int	redirect_pipe(int pipefd[2], char type)
{
	if (type == 'r')
	{	
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
			return (-1);
		close(pipefd[1]);
	}
	else if (type == 'w')
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			return (-1);
		close(pipefd[0]);
	}
	return (0);
}

int ft_popen(const char *file, char *const argv[], char type)
{
	int	pipefd[2];
	int	pid;

	// Setup Pipe
	if (pipe(pipefd) != 0)
		return (-1);
		
	// Fork a process
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}

	// Execute child process
	if (pid == 0)
	{
		// Redirect Pipe
		if (redirect_pipe(pipefd, type) != 0)
			exit(-1);
		execvp(file, argv);
		// Exit on error
		exit(-1);
	}

	if (type == 'r')
	{
		close(pipefd[1]);
		return (pipefd[0]);
	}
	else if (type == 'w')
	{
		close(pipefd[0]);
		return (pipefd[1]);
	}
	return (-1);
}

#include <stdio.h>
int main() {
    FILE *fp;
    char buffer[128];

    // Open the 'ls -l' command for reading its output
    fp = popen("ls -l", "r");
    if (fp == NULL) {
        perror("popen failed");
        exit(EXIT_FAILURE);
    }

    // Read and print the output line by line
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    // Close the pipe and get the command's exit status
    int status = pclose(fp);
    if (status == -1) {
        perror("pclose failed");
    } else {
        printf("Command exited with status %d\n", WEXITSTATUS(status));
    }

    return 0;
}
