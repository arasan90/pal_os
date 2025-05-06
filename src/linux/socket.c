/*
 * File: socket.c
 * Description: Implementation of socket-related functionality for the Linux platform.
 * Author: Massimiliano Ianniello
 */

#include "pal_os/socket.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/* ---------------------------------------------------------------------------
 * Type Definitions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Static Definitions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Macros
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Constants
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Static Functions
 * ---------------------------------------------------------------------------
 */

/* ---------------------------------------------------------------------------
 * Function Implementations
 * ---------------------------------------------------------------------------
 */

int pal_socket_create(pal_socket_type_t type, pal_socket_protocol_t protocol, int reuse_addr)
{
	int socket_fd		= -1;
	int socket_type		= (PAL_SOCKET_TYPE_STREAM == type) ? SOCK_STREAM : (PAL_SOCKET_TYPE_DGRAM == type) ? SOCK_DGRAM : -1;
	int socket_protocol = (PAL_SOCKET_PROTOCOL_TCP == protocol) ? IPPROTO_TCP : (PAL_SOCKET_PROTOCOL_UDP == protocol) ? IPPROTO_UDP : -1;
	if (-1 != socket_type && -1 != socket_protocol)
	{
		socket_fd = socket(AF_INET, socket_type, socket_protocol);
		if (socket_fd >= 0)
		{
			if (reuse_addr)
			{
				int optval = 1;
				setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
			}
		}
	}
	return socket_fd;
}

int pal_socket_bind(int socket, const char *address, int port)
{
	int				   ret_code = -1;
	struct sockaddr_in addr		= {0};
	addr.sin_family				= AF_INET;
	addr.sin_port				= htons(port);
	if (socket > 0 && inet_pton(AF_INET, address, &addr.sin_addr) && 0 == bind(socket, (struct sockaddr *)&addr, sizeof(addr)))
	{
		ret_code = 0;
	}
	return ret_code;
}

int pal_socket_listen(int socket, int max_connections)
{
	int ret_code = -1;
	if (socket >= 0 && max_connections > 0)
	{
		ret_code = (0 == listen(socket, max_connections)) ? 0 : -1;
	}
	return ret_code;
}

int pal_socket_accept(int socket)
{
	int socket_fd = -1;
	if (socket >= 0)
	{
		socket_fd = accept(socket, NULL, NULL);
	}
	return socket_fd;
}

int pal_socket_connect(int socket, const char *address, int port)
{
	int				   ret_code = -1;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port	= htons(port);
	if (socket >= 0 && inet_pton(AF_INET, address, &addr.sin_addr) && 0 == connect(socket, (struct sockaddr *)&addr, sizeof(addr)))
	{
		ret_code = 0;
	}
	return ret_code;
}

ssize_t pal_socket_send(int socket, const void *buffer, size_t length)
{
	int bytes_sent = -1;
	if (socket >= 0 && buffer != NULL && length > 0)
	{
		bytes_sent = send(socket, buffer, length, 0);
	}
	return bytes_sent;
}

ssize_t pal_socket_receive(int socket, void *buffer, size_t size)
{
	ssize_t bytes_received = -1;
	if (socket >= 0 && buffer != NULL && size > 0)
	{
		bytes_received = recv(socket, buffer, size, 0);
	}
	return bytes_received;
}

int pal_socket_close(int socket)
{
	int ret_code = -1;
	if (socket >= 0)
	{
		ret_code = (0 == close(socket)) ? 0 : -1;
	}
	return ret_code;
}