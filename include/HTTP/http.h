#ifndef HTTP_HTTP_H
#define HTTP_HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <helpers/helpers.h>

#define MAX_MSG 1024

/**
 * \brief generate standard get message
 * \param components URL components parsed with url_parser() function
 * \return char * get message
 */
char *generate_get_request_message(struct URL_Components* components);

/**
 * \brief generate standard post message
 * \param components URL components parsed with url_parser() function
 * \param headers headers to be used on the post request. obs: each headers need to end with "\r\n"
 * \param data data to be used on the post request
 * \param data_len length of the data that will be used on the post request
 * \return char * post message
 */
char *generate_post_request_message(
  struct URL_Components* components,
  char *headers,
  char *data,
  size_t data_len
);

/**
 * \brief creates and initates the socket that will make the request
 * \param components URL components parsed with url_parser() function
 * \param message to be sent in the request
 * \return 0 if success otherwise will end the program
 */
int make_http_request(struct URL_Components* components, char *message);

#endif