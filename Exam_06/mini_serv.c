#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/select.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>  // Essential for structures and byte ordering
#include <netdb.h>

#define true 1
#define false 0

void	broadcast(int sender_fd, int server_fd, int max_fd, fd_set* master_fd, const char* msg)
{
	for (int i = 0; i <= max_fd; i++)
	{
		// Check if the socket is active, AND is not the server, AND is not the sender
		if (FD_ISSET(i, master_fd) && i != server_fd && i != sender_fd)
		{
			send(i, msg, strlen(msg), 0);
		}
	}
}


int	multiplexer(int sockefd)
{
	fd_set	master_fd, read_fd;
	FD_ZERO(&master_fd);
	FD_SET(sockefd, &master_fd);

	int	max_fd = sockefd;
	char buffer[1024];
    char msg[2048]; // Buffer to format our broadcast messages

	while (true)
	{
		read_fd = master_fd;
		if (select(max_fd + 1, &read_fd, NULL, NULL, NULL) == -1)
			continue;
		printf("Events: %d\n", max_fd);
		for (int event = 0; event <= max_fd; event++)
		{
			if (FD_ISSET(event, &read_fd))
			{
				// 1. A new client is trying to connect
				if (event == sockefd)
				{
					int client_fd = accept(sockefd, NULL, NULL);
					if (client_fd >= 0)
					{
						FD_SET(client_fd, &master_fd);
						if (client_fd > max_fd)
							max_fd = client_fd;

						sprintf(msg, "server: client %d just arrived\n", client_fd - 4);
						broadcast(client_fd, sockefd, max_fd, &master_fd, msg);
					}
				}
				// 2. An existing client is sending data or disconnecting
				else
				{
					int read_bytes = recv(event, buffer, sizeof(buffer) - 1, 0);
					if (read_bytes <= 0)
					{
						sprintf(msg, "server: client %d just left\n", event - 4);
						broadcast(event, sockefd, max_fd, &master_fd, msg);
						close(event);
						FD_CLR(event, &master_fd);
					}
					else
					{
						buffer[read_bytes] = '\0';
						sprintf(msg, "client %d: %s", event - 4, buffer);
						broadcast(event, sockefd, max_fd, &master_fd, msg);
					}
				}
			}
		}
	}
	return (EXIT_SUCCESS);
}

int main(int arc, char* arv[])
{
	if (arc != 2)
	{
		write(STDERR_FILENO, "Wrong number of arguments\n", 26);
		return (EXIT_FAILURE);
	}

	struct addrinfo	hints;
	struct addrinfo	*result;
	struct addrinfo *cur;
	int				sockefd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	// hints.[...] = 0 | NULL;

	int	s = getaddrinfo("127.0.0.1", arv[1], &hints, &result);
	if (s != 0)
	{
		write(STDERR_FILENO, "getaddrinfo failed!\n", 20);
		return (EXIT_FAILURE);
	}

	for (cur = result; cur != NULL; cur = cur->ai_next)
	{
		sockefd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
		if (sockefd == -1)
		{
			continue;
		}
		if (bind(sockefd, cur->ai_addr, cur->ai_addrlen) == 0)
			break;
		close(sockefd);
	}
	freeaddrinfo(result);

	if (cur == NULL)
	{
		write(STDERR_FILENO, "Cound not bind any address\n", 27);
		return (EXIT_FAILURE);
	}

	if (listen(sockefd, 1024) != 0)
	{
		write(STDERR_FILENO, "Cound not listen\n", 17);
		close(sockefd);
		return (EXIT_FAILURE);
	}

	if (multiplexer(sockefd) != EXIT_SUCCESS)
	{
		close(sockefd);
		close(sockefd);
		return (EXIT_FAILURE);
	}

	close(sockefd);
	return (EXIT_SUCCESS);
}
