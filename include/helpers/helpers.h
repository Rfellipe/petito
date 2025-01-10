#ifndef HELPERS_H
#define HELPERS_H

#define ARR_SIZE(x) (sizeof(x) / (sizeof(x)[0]))

struct URL_Components {
  char* scheme;
  char* host;
  char* port;
  char* path;
  char* query;
  char* fragment;
};

/**
 * \brief Get the full path of URL components
 *
 * \param components URL components parsed with url_parser() function
 * \retval NULL if no components or no path and no query passed
 * \retval char* path if no query passed ex: users/info
 * \retval char* leading query ex: ?user=admin
 * \retval char* path and query together ex: users/info/?user=admin
 */
char* get_full_path(const struct URL_Components* components);


/**
 * \brief Get the host with port
 *
 * \param components URL components parsed with url_parser() function
 * \retval NULL if no components or no path passed
 * \retval char* host if no port passed ex: gnu.org
 * \retval char* host and port together ex: localhost:3000
 */
char* get_host_with_port(const struct URL_Components* components);

// parse url given in arguments
struct URL_Components* url_parser(char *url);

// free the memory in the component struct
void free_url_components(struct URL_Components* components);

void print_url_components(struct URL_Components* components);

void upper_string(char *str);

#endif
