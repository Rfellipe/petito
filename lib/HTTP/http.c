#include <HTTP/http.h>
#include <socket/handlers.h>
#include <helpers/helpers.h>

char *generate_get_request_message(struct URL_Components* components) {
  char *full_path = get_full_path(components);
  char *host_with_port = get_host_with_port(components);
  char *buffer = malloc(MAX_MSG);
  if (!buffer)
    return NULL;

  const char* format = ("GET /%s HTTP/1.1\r\n"
    "User-Agent: api_tester/0.1\r\n"
    "Host: %s\r\n"
    "Accept: */*\r\n"
    "\r\n"
  );

  sprintf(buffer, format, full_path, host_with_port);

  return buffer;
}

char *generate_post_request_message(
  struct URL_Components* components,
  char *headers,
  char *data,
  size_t data_len
) {
  char *full_path = get_full_path(components);
  char *host_with_port = get_host_with_port(components);
  char *buffer = malloc(MAX_MSG);
  if (!buffer)
    return NULL;

  const char* format = ("POST /%s HTTP/1.1\r\n"
    "Host: %s\r\n"
    "User-Agent: api_tester/0.1\r\n"
    "Accept: */*\r\n"
    "Content-Length: %d\r\n"
    "Connection: close\r\n"
    "%s"
    "\r\n"
    "%s"
  );

  sprintf(buffer, format, full_path, host_with_port, data_len, headers, data);

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
