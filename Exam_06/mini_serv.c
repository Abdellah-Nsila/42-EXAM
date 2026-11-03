#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

fd_set original_set, read_set, write_set;
int max_fd = 0;
int ids[4096];
int count = 0;
char* msgs[4096];

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
	exit(EXIT_FAILURE);
}

// broadcast to all client (custom msg)
void broadcast_to_all(int except_id, char* msg)
{
	for (int client_fd = 0; client_fd <= max_fd; client_fd++)
	{
		if (FD_ISSET(client_fd, &write_set) && client_fd != except_id)
		{
			send(client_fd, msg, strlen(msg), 0);
		}
	}
}

// remove client
int remove_client(int event)
{
	char msg[128];

	// Clear remained buffer
	if (msgs[event] != NULL)
	{
		free(msgs[event]);
		msgs[event] = NULL;
	}
	sprintf(msg, "server: client %d just left\n", ids[event]);
	broadcast_to_all(event, msg);
	close(event);
	FD_CLR(event, &original_set);
	return (EXIT_SUCCESS);
}

// add client
int connect_client(int sockfd)
{
	char msg[128];

	int client_fd = accept(sockfd, NULL, NULL);
	if (client_fd >= 0)
	{
		FD_SET(client_fd, &original_set);
		if (client_fd > max_fd)
			max_fd = client_fd;
		// Set current client id and Increment count for next client
		ids[client_fd] = count++;
		sprintf(msg, "server: client %d just arrived\n", ids[client_fd]);
		broadcast_to_all(client_fd, msg);
	}
	return (EXIT_SUCCESS);
}

// handle client
int handle_client(int event)
{
	char msg[4096];
	char header[128];
	ssize_t	nbytes = 0;

	nbytes = recv(event, msg, sizeof(msg) - 1, 0);
	if (nbytes <= 0)
	{
		return (remove_client(event));
	}
	msg[nbytes] = '\0';

	char* ligne = NULL;
	msgs[event] = str_join(msgs[event], msg);

	while (extract_message(&msgs[event], &ligne))
	{	
		sprintf(header, "client %d: ", ids[event]);
		broadcast_to_all(event, header);
		broadcast_to_all(event, ligne);
		free(ligne);
	}
	return (EXIT_SUCCESS);
}

int	monitor(int sockfd)
{
	// select()
	max_fd = sockfd;

	FD_ZERO(&original_set);
	// add sockfd to select original_set
	FD_SET(sockfd, &original_set);

	while (1)
	{
		read_set = write_set = original_set;
		int events = select(max_fd + 1, &read_set, &write_set, NULL, NULL);
		if (events == -1)
			fatal_error();
		for (int event = 0; event <= max_fd; event++)
		{
			// 1 new client
			if (!FD_ISSET(event, &read_set))
				continue;
			if (event == sockfd)
			{
				connect_client(sockfd);
				break;
			}
			// 2 exising client sending a msg
			// 3 OR client left
			else
			{
				handle_client(event);
			}
		}
	}
}

int main(int ac, char* av[])
{
	int sockfd;
	struct sockaddr_in servaddr;

	if (ac != 2)
	{
		write(STDERR_FILENO, "Wrong number of arguments\n", 26);
		exit(EXIT_FAILURE);
	}

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		fatal_error();
	}
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); // convert port to int first

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
	{
		fatal_error();
	}
	if (listen(sockfd, SOMAXCONN) != 0)
	{
		fatal_error();
	}

	return (monitor(sockfd));
}
