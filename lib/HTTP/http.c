#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <HTTP/http.h>
#include <socket/handlers.h>
#include <helpers/helpers.h>


char *generate_request_message(struct URL_Components* components) {
  char *buffer = malloc(MAX_MSG);
  if (!buffer)
    return NULL;

  const char* format = ("%s /%s?%s HTTP/1.1\r\n"
    "User-Agent: api_tester/0.1\r\n"
    "Host: %s\r\n"
    "Accept: */*\r\n"
    "\r\n");

  if (components->port) {
    char* host_with_port;
    strcpy(host_with_port, components->host);
    strcat(host_with_port, ":");
    strcat(host_with_port, components->port);
    sprintf(buffer, format, "GET", components->path, components->query, host_with_port);
  } else {
    sprintf(buffer, format, "GET", components->path, components->query, components->host);
  }

  return buffer;
}

int read_from_client(int filedes) {
  char buffer[MAX_MSG];
  int nbytes;

  while (1) {
    memset(&buffer, 0, MAX_MSG);
    nbytes = read(filedes, buffer, MAX_MSG);
    if (nbytes < 0) {
      /* Read error. */
      perror ("read");
      exit (EXIT_FAILURE);
    } else if (nbytes == 0) {
      /* End-of-file. */
      return -1;
    } else {
      /* Data read. */
      printf("%s", buffer);
      return 0;
    }
  }
}

int make_get_request(struct URL_Components* components, char *message) {
  int sock, port, nbytes;
  socklen_t size;
  struct sockaddr_in servername;

  if (components->port) {
    port = strtol(components->port, NULL, 10);
  } else {
    port = 80;
  }

  sock = create_socket();
  init_sockaddr(&servername, components->host, port);
  if (connect(sock, (struct sockaddr *)&servername, sizeof(servername)) < 0) {
    perror("Client connection error");
    exit(1);
  }

  free_url_components(components);

  nbytes = write(sock, message, strlen(message) + 1);
  if (nbytes < 0) {
    perror("Error writing message to socket");
    exit(EXIT_FAILURE);
  }

  while(read_from_client(sock) != 0) {
    continue;
  }

  return 0;
}