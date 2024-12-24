#ifndef SOCKET_HANDLERS_H
#define  SOCKET_HANDLERS_H

#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/bio.h>

/**
 * Initializes a socket address structure with the given hostname and port.
 *
 * @param name     Pointer to sockaddr_in structure to be initialized
 * @param hostname The host name or IP address to connect to
 * @param port     The port number to connect to
 *
 * @note This function will terminate the program if the hostname cannot be resolved
 */
void init_sockaddr(struct sockaddr_in *name,
                  const char *hostname,
                  uint16_t port);

/**
 * Creates a TCP/IP socket.
 *
 * Creates a new socket using TCP/IP protocol (PF_INET, SOCK_STREAM).
 *
 * @return The socket file descriptor on success
 * @note This function will terminate the program if socket creation fails
 */
int create_socket(void);

/**
 * Reads data from a socket until a double CRLF is encountered.
 *
 * Reads data in chunks into a buffer until either:
 * - A double CRLF sequence ("\r\n\r\n") is found
 * - The buffer is full (4096 bytes)
 * - The connection is closed
 *
 * @param sock Socket file descriptor to read from
 *
 * @note The received data is printed to stdout
 * @note Maximum buffer size is 4096 bytes
 */
void read_from_socket(int sock);

void read_from_BIO_socket(SSL *ssl);

BIO *create_bio_socket(char *hostname, char *port);

#endif
