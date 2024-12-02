#include <stdio.h>
#include <helpers/helpers.h>
#include <HTTP/http.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int ret;
  char *message, *headers, *data;

  struct URL_Components* components = url_parser(argv[2]);

  if (strcmp(argv[1], "GET") == 0 || strcmp(argv[1], "get") == 0 ) {
    message = generate_get_request_message(components);
  }

  if (strcmp(argv[1], "POST") == 0 || strcmp(argv[1], "post") == 0 ) {
    // int size_arg = sizeof(argv[3]);
    // memcpy(headers, argv[3], size_arg);
    // printf("%s\n", headers);
    data = argv[argc - 1];
    message = generate_post_request_message(components, "", data, strlen(data));
    printf("%s", message);
  }

  ret = make_http_request(components, message);
  if (ret != 0) {
    perror("Error making request");
  }

  exit(0);
}