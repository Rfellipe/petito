#ifndef HTTP_HTTP_H
#define HTTP_HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <helpers/helpers.h>

#define MAX_MSG 1024

enum http_verbs {
  GET,
  POST,
  UPDATE,
  DELETE,
  UNDEFINED
};

char *generate_get_request_message(struct URL_Components* components);

char *generate_post_request_message(
  struct URL_Components* components,
  char *headers,
  char *data,
  size_t data_len
);

int make_http_request(struct URL_Components* components, char *message);

#endif