#include <HTTP/http.h>
#include <socket/handlers.h>
#include <helpers/helpers.h>

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

char *generate_get_request_message(struct URL_Components* components) {
  char *buffer = malloc(MAX_MSG);
  if (!buffer)
    return NULL;

  const char* format = ("GET /%s?%s HTTP/1.1\r\n"
    "User-Agent: api_tester/0.1\r\n"
    "Host: %s\r\n"
    "Accept: */*\r\n"
    "\r\n");

  if (components->port) {
    char* host_with_port;
    strcpy(host_with_port, components->host);
    strcat(host_with_port, ":");
    strcat(host_with_port, components->port);
    sprintf(buffer, format, components->path, components->query, host_with_port);
  } else {
    sprintf(buffer, format,  components->path, components->query, components->host);
  }

  return buffer;
}

char *generate_post_request_message(
  struct URL_Components* components,
  char *headers,
  char *data,
  size_t data_len
) {
  char *buffer = malloc(MAX_MSG);
  if (!buffer)
    return NULL;

  const char* format = ("POST /%s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "User-Agent: api_tester/0.1\r\n"
    "Accept: */*\r\n"
    "Content-Length: %d\r\n"
    "Content-Type: application/json\r\n"
    "Connection: close\r\n"
    "%s"
    "\r\n"
    "%s");

  if (components->port) {
    char* host_with_port;
    strcpy(host_with_port, components->host);
    strcat(host_with_port, ":");
    strcat(host_with_port, components->port);
    sprintf(buffer, format, components->path, host_with_port, data_len, headers, data);
  } else {
    sprintf(buffer, format,  components->path, components->host, data_len, headers, data);
  }

  return buffer;
}

int make_http_request(struct URL_Components* components, char *message) {
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

  read_from_socket(sock);

  return 0;
}
