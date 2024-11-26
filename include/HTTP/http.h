#ifndef HTTP_HTTP_H
#define HTTP_HTTP_H

#include <helpers/helpers.h>

#define MAX_MSG 1024

enum http_verbs {
  GET,
  POST,
  UPDATE,
  DELETE,
  UNDEFINED
};

char *generate_request_message(struct URL_Components* components);

int make_get_request(struct URL_Components* components, char *message);

#endif