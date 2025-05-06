#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <sys/types.h>

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
typedef enum pal_socket_type_e
{
	PAL_SOCKET_TYPE_STREAM = 0,	 //!< Stream socket
	PAL_SOCKET_TYPE_DGRAM  = 1,	 //!< Datagram socket
} pal_socket_type_t;

typedef enum pal_socket_protocol_e
{
	PAL_SOCKET_PROTOCOL_TCP = 0,  //!< TCP protocol
	PAL_SOCKET_PROTOCOL_UDP = 1,  //!< UDP protocol
} pal_socket_protocol_t;

// ============================
// Function Declarations
// ============================

/**
 * @brief Creates a socket.
 *
 * @param type The socket type (e.g., PAL_SOCKET_TYPE_STREAM or PAL_SOCKET_TYPE_DGRAM).
 * @param protocol The socket protocol (e.g., PAL_SOCKET_PROTOCOL_TCP or PAL_SOCKET_PROTOCOL_UDP).
 * @param reuse_addr Specifies whether the address can be reused (1) or not (0).
 * @return The socket descriptor on success, or -1 on error.
 */
int pal_socket_create(pal_socket_type_t type, pal_socket_protocol_t protocol, int reuse_addr);

/**
 * @brief Binds a socket to a specific address and port.
 *
 * @param socket The socket descriptor.
 * @param address The IP address to bind the socket to.
 * @note Only IPV4 addresses are supported.
 * @param port The port to bind the socket to.
 * @return 0 on success, or -1 on error.
 */
int pal_socket_bind(int socket, const char *address, int port);

/**
 * @brief Puts the socket into listening mode for incoming connections.
 *
 * @param socket The socket descriptor.
 * @param max_connections The maximum number of pending connections in the queue.
 * @return 0 on success, or -1 on error.
 */
int pal_socket_listen(int socket, int max_connections);

/**
 * @brief Accepts an incoming connection on a listening socket.
 *
 * @param socket The socket descriptor.
 * @return The descriptor for the accepted connection on success, or -1 on error.
 */
int pal_socket_accept(int socket);

/**
 * @brief Connects a socket to a remote address and port.
 *
 * @param socket The socket descriptor.
 * @param address The remote IP address to connect to.
 * @note Only IPV4 addresses are supported.
 * @param port The remote port to connect to.
 * @return 0 on success, or -1 on error.
 */
int pal_socket_connect(int socket, const char *address, int port);

/**
 * @brief Sends data through a socket.
 *
 * @param socket The socket descriptor.
 * @param buffer The buffer containing the data to send.
 * @param length The length of the data to send.
 * @return The number of bytes sent on success, or -1 on error.
 */
ssize_t pal_socket_send(int socket, const void *buffer, size_t length);

/**
 * @brief Receives data from a socket.
 *
 * @param socket The socket descriptor.
 * @param buffer The buffer to store the received data.
 * @param size The maximum size of the buffer.
 * @return The number of bytes received on success, or -1 on error.
 */
ssize_t pal_socket_receive(int socket, void *buffer, size_t size);

/**
 * @brief Closes a socket.
 *
 * @param socket The socket descriptor.
 * @return 0 on success, or -1 on error.
 */
int pal_socket_close(int socket);

#ifdef __cplusplus
}
#endif
