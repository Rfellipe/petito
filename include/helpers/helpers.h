#ifndef HELPERS_H
#define HELPERS_H

struct URL_Components {
  char* scheme;
  char* host;
  char* port;
  char* path;
  char* query;
  char* fragment;
};

char* get_full_path(const struct URL_Components* components);

char* get_host_with_port(const struct URL_Components* components);

// parse url given in arguments
struct URL_Components* url_parser(char *url);

// free the memory in the component struct
void free_url_components(struct URL_Components* components);

#endif