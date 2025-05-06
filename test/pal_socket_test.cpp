#include <gtest/gtest.h>

#include <thread>

#include "pal_os/socket.h"

TEST(pal_os_socket, createStreamSocketSuccess)
{
	int socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
	EXPECT_GE(socket_fd, 0);
	pal_socket_close(socket_fd);
}

TEST(pal_os_socket, createDgramSocketSuccess)
{
	int socket_fd = pal_socket_create(PAL_SOCKET_TYPE_DGRAM, PAL_SOCKET_PROTOCOL_UDP, 1);
	EXPECT_GE(socket_fd, 0);
	pal_socket_close(socket_fd);
}

TEST(pal_os_socket, createInvalidSocketType)
{
	int socket_fd = pal_socket_create(PAL_SOCKET_TYPE_DGRAM, PAL_SOCKET_PROTOCOL_TCP, 1);
	EXPECT_EQ(-1, socket_fd);
	socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_UDP, 1);
	EXPECT_EQ(-1, socket_fd);
}

TEST(pal_os_socket, bindSuccess)
{
	int socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
	EXPECT_GE(socket_fd, 0);
	EXPECT_EQ(0, pal_socket_bind(socket_fd, "127.0.0.1", 49156));
	pal_socket_close(socket_fd);
}

TEST(pal_os_socket, bindWrongAddressFailure)
{
	int socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
	EXPECT_GE(socket_fd, 0);
	EXPECT_EQ(-1, pal_socket_bind(socket_fd, "132.0.0.1", 49156));
	pal_socket_close(socket_fd);
}

TEST(pal_os_socket, acceptIncomingConnection)
{
	int server_socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
	pal_socket_bind(server_socket_fd, "127.0.0.1", 49157);
	ASSERT_EQ(0, pal_socket_listen(server_socket_fd, 1));

	std::thread client_thread(
		[]()
		{
			int client_socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
			ASSERT_GE(client_socket_fd, 0);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			ASSERT_EQ(0, pal_socket_connect(client_socket_fd, "127.0.0.1", 49157));
			pal_socket_close(client_socket_fd);
		});

	int accepted_socket_fd = pal_socket_accept(server_socket_fd);
	EXPECT_GE(accepted_socket_fd, 0);

	client_thread.join();
	pal_socket_close(accepted_socket_fd);
	pal_socket_close(server_socket_fd);
}

TEST(pal_os_socket, sendAndReceiveData)
{
	int server_socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
	pal_socket_bind(server_socket_fd, "127.0.0.1", 49158);
	pal_socket_listen(server_socket_fd, 1);

	std::thread client_thread(
		[]()
		{
			int client_socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			pal_socket_connect(client_socket_fd, "127.0.0.1", 49158);

			const char* message	   = "Hello, Server!";
			ssize_t		bytes_sent = pal_socket_send(client_socket_fd, message, strlen(message));
			EXPECT_EQ(bytes_sent, strlen(message));

			pal_socket_close(client_socket_fd);
		});

	int accepted_socket_fd = pal_socket_accept(server_socket_fd);

	char	buffer[1024]   = {0};
	ssize_t bytes_received = pal_socket_receive(accepted_socket_fd, buffer, sizeof(buffer) - 1);
	EXPECT_GT(bytes_received, 0);
	EXPECT_EQ(bytes_received, strlen("Hello, Server!"));
	EXPECT_STREQ(buffer, "Hello, Server!");

	client_thread.join();
	pal_socket_close(accepted_socket_fd);
	pal_socket_close(server_socket_fd);
}

TEST(pal_os_socket, closeSuccess)
{
	int socket_fd = pal_socket_create(PAL_SOCKET_TYPE_STREAM, PAL_SOCKET_PROTOCOL_TCP, 1);
	EXPECT_EQ(0, pal_socket_close(socket_fd));
}

TEST(pal_os_socket, closeInvalidSocketFailure) { EXPECT_EQ(-1, pal_socket_close(-1)); }

TEST(pal_os_socket, closeNotOpenedSocketFailure) { EXPECT_EQ(-1, pal_socket_close(34)); }
