#include <stdio.h>
#include <helpers/helpers.h>
#include <HTTP/http.h>

int main(int argc, char **argv) {
  int ret;

  struct URL_Components* components = url_parser(argv[1]);

  char *message = generate_request_message(components);

  ret = make_get_request(components, message);
  if (ret != 0) {
    perror("Error making request");
  }
}