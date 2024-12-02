#ifndef SOCKET_HANDLERS_H
#define  SOCKET_HANDLERS_H

#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * This function sole purpose to create a socket
 * at the start of the main.c file.
 **/
int create_socket();


/**
 * Function to connect to the server.
 */
void init_sockaddr(struct sockaddr_in *name,
                  const char *hostname,
                  uint16_t port);


void read_from_socket(int sock);

#endif