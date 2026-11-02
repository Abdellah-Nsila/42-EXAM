#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

int extract_message(char **buf, char **msg)
{
	char *newbuf;
	int i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char *newbuf;
	int len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void fatal_error()
{
	write(STDERR_FILENO, "Fatal error\n", 12);
}

// add client
int connect_client(fd_set *read_set, int nfds, int sockfd)
{
	char msg[128];

	int client_fd = accept(sockfd, NULL, NULL);
	if (client_fd < 0)
		return (EXIT_FAILURE);
	FD_SET(client_fd, read_set);
	sprintf(msg, "server: client %d just arrived\n", client_fd);
	return (broadcast_to_all(read_set, nfds, client_fd, msg));
}

// handle client
int handle_client(fd_set *read_set, int nfds, int event)
{
	char buffer[1024];
	char msg[2048];
	ssize_t	nbytes = 0;

	nbytes = recv(event, buffer, sizeof(buffer) - 1, 0);
	if (nbytes <= 0)
	{
		return (remove_client(read_set, nfds, event));
	}
	sprintf(buffer, "client %d: %s\n", event, msg);
	return (broadcast_to_all(read_set, nfds, event, msg));
}

// remove client
int remove_client(fd_set *read_set, int nfds, int event)
{
	char msg[128];

	Fd_SET(event, read_set);
	sprintf(msg, "server: client %d just left\n", event);
	broadcast_to_all(read_set, nfds, event, msg);
	close(event);
	FD_CLR(event, read_set);
}

// broadcast to all client (custom msg)
int broadcast_to_all(fd_set *read_set, int nfds, int except_id, char* msg)
{
	for (int i = 0; i < nfds; i++)
	{
		if (FD_ISSET(i, read_set) && i != except_id)
		{
			if (send(i, msg, strlen(msg), 0) < 0)
				return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	monitor(int sockfd)
{
	// select()
	int nfds = sockfd;
	fd_set	read_set, tmp_set;
	fd_set	write_set;
	fd_set	except_set;
	struct timeval	tv;

	FD_ZERO(&read_set);
	// add sockfd to select read_set
	FD_SET(sockfd, &read_set);

	tv.tv_sec = 5;
	tv.tv_usec = 0;

	while (1)
	{
		tmp_set = read_set;
		int events = select(nfds + 1, &tmp_set, NULL, NULL, &tv);
		if (events == -1)
			continue;
		for (int event = 0; event <= nfds; event++)
		{
			// 1 exising client sending a msg
			if (FD_ISSET(event, &read_set))
			{
				// client %d: msg
				if (handle_client(&read_set, nfds, event) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
			// 2 new client
			else if (event == sockfd)
			{
				// server: client %d just arrived\n
				if (connect_client(&read_set, nfds, event) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
			// 3 client left
			else
			{
				// "server: client %d just left\n"
				if (remove_client(&read_set, nfds, event) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
		}

	}
}

int main(int ac, char* av[])
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	if (ac != 2)
	{
		write(STDERR_FILENO, "Wrong number of arguments\n", 26);
		return (EXIT_FAILURE);
	}

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		fatal_error();
		exit(EXIT_FAILURE);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
	servaddr.sin_port = htons(8081);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		fatal_error();
		exit(EXIT_FAILURE);
	}
	else
		printf("Socket successfully binded..\n");
	if (listen(sockfd, 10) != 0)
	{
		fatal_error();
		exit(EXIT_FAILURE);
	}
	len = sizeof(cli);
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (connfd < 0)
	{
		fatal_error();
		exit(EXIT_FAILURE);
	}
	else
		printf("server acccept the client...\n");
	
	monitor(sockfd);


}