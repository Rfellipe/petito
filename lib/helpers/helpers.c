#include <helpers/helpers.h>
#include <string.h>
#include <stdlib.h>

struct URL_Components* url_parser(char *url) {
  struct URL_Components* components = calloc(1, sizeof(struct URL_Components));
  char* temp = strdup(url);
  char* cursor = temp;

  // Parser scheme
  char* scheme_end = strstr(cursor, "://");
  if (scheme_end) {
    *scheme_end = '\0';
    components->scheme = strdup(cursor);
    cursor = scheme_end + 3;
  }

  // Parser fragment
  char* fragment = strchr(cursor, '#');
  if (fragment) {
    *fragment = '\0';
    components->fragment = strdup(fragment + 1);
  }

  // Parse query
  char* query = strchr(cursor, '?');
  if (query) {
    *query = '\0';
    components->query = strdup(query + 1);
  }

  // Parse host and port
  char* path = strchr(cursor, '/');
  if (path) {
    *path = '\0';
    components->path = strdup(path + 1);
  }

  char* port = strchr(cursor, ':');
  if (port) {
    *port = '\0';
    components->port = strdup(port + 1);
  }

  components->host = strdup(cursor);

  // printf("/%s?%s", components->path, components->query);

  free(temp);
  return components;
}

void free_url_components(struct URL_Components* components) {
  if (components) {
    free(components->scheme);
    free(components->host);
    free(components->port);
    free(components->path);
    free(components->query);
    free(components->fragment);
    free(components);
  }
}