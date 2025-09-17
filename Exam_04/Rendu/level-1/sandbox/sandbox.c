#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <errno.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p = (unsigned char *)s;

	while (n--)
		*p++ = (unsigned char)c;
	return (s);
}

void	alarm_handler(int sig)
{
	(void)sig;
}

void	set_signal_action()
{
	struct sigaction act;

	ft_memset((void *)&act, 0, sizeof(act));
	act.sa_handler = &alarm_handler;
	sigaction(SIGALRM, &act, NULL);
}

int	signal_notif(pid_t pid, int *status, bool verbose, unsigned int timeout)
{
	int	exit_code;

	if (waitpid(pid, status, 0) == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
			return (0);
		}
		return (-1);
	}
	if (WIFSIGNALED(*status))
	{
		if (verbose)
			printf("Bad function: %s\n", strsignal(WTERMSIG(*status)));
		return (0);
	}
	else if (WIFEXITED(*status))
	{
		exit_code = WEXITSTATUS(*status);
		if (exit_code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		else
		{
			if (verbose)
				printf("Bad function: exited with code %d\n", exit_code);
			return (0);
		}
	}
	else
		return (-1);
}

void	child(void (*f)(void))
{
	f();
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status;

	if (!f)
		return (-1);
	set_signal_action();
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		child(f);
		exit(EXIT_SUCCESS);
	}
	alarm(timeout);
	return(signal_notif(pid, &status, verbose, timeout));
}

// // 1. Nice function - should return successfully
// void nice_function(void)
// {
// 	// This function does nothing bad
// 	return;
// }

// // 2. Bad function - exits with code 1
// void bad_function_exit_1(void)
// {
// 	exit(1);
// }

// void bad_function_segfault(void)
// {
//     int *ptr = NULL;
//     *ptr = 42;  // Causes segfault
// }

// void bad_function_abort(void)
// {
//     abort();
// }

// void bad_function_infinite_loop(void)
// {
//     while (1) {
//         // This will timeout
//     }
// }

// int		main()
// {
// 	sandbox(&nice_function, 1, true);
// 	sandbox(&bad_function_exit_1, 1, true);
// 	sandbox(&bad_function_segfault, 1, true);
// 	sandbox(&bad_function_abort, 1, true);
// 	sandbox(&bad_function_infinite_loop, 1, true);
// }
