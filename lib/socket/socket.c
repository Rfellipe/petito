#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <socket/handlers.h>

void init_sockaddr(struct sockaddr_in *name,
                  const char *hostname,
                  uint16_t port)
{
  struct hostent *hostinfo;

  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL) {
    fprintf(stderr, "Unknown host %s.\n", hostname);
    exit(EXIT_FAILURE);
  }
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}

int create_socket() {
  int ret;

  ret = socket(PF_INET, SOCK_STREAM, 0);
  if (ret < 0) {
    printf("Error on creating socket\n");
    exit(EXIT_FAILURE);
  }

  return ret;
}