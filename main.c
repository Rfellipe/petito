#include <HTTP/http.h>
#include <argp.h>
#include <helpers/helpers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int verbose_flag;
static int post_flag;

const char *argp_program_version = "petito-1.0.1";
const char *argp_program_bug_address = "<ribeirofellipe13@outlook.com>";

// Program documentation
static char doc[] = "petito -- Another program to make request";

// Arguments description
static char args_doc[] = "<url>";

//{"url",      'u', "STRING", 0, "Specify url (optional)"},
static struct argp_option opts[] = {
    {"verbose", 'v', 0, 0, "Show more information on output"},
    {"method", 'x', "STRING", 0, "Specify request method"},
    {"data", 'd', "STRING", 0, "HTTP POST data"},
    {"header", 'H', "STRING", 0, "Add header to request"},
    {"file", 'f', "FILE", 0, "Make request based on file"},
    {"template", 't', "FILE", 0, "Generate template file to make requests"},
    {"output", 'o', "FILE", 0, "Write output to a file insted of stdo"},
    {0}};

struct arguments {
  int verbose, url_count;
  char *data, *headers, *method;
  FILE *file, *template;
  FILE *output;
  char **url;
};

static error_t parse_opts(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
  case 'v':
    arguments->verbose = 1;
    printf("verbosity activated\n");
    break;

  case 'H':
    strcat(arguments->headers, arg);
    strcat(arguments->headers, "\r\n");
    break;

  case 'x':
    arguments->method = strdup(arg);
    upper_string(arguments->method);
    break;

  case 'd':
    if ((strcmp(arguments->method, "GET") == 0) ||
        strcmp(arguments->method, "HEAD") == 0) {
      arguments->method = "POST";
    }
    arguments->data = strdup(arg);
    break;

  case ARGP_KEY_ARG:
    arguments->url_count++;
    arguments->url[state->arg_num] = (char *)malloc((512 + 1) * sizeof(char));
    arguments->url[state->arg_num] = arg;
    break;

  case ARGP_KEY_END:
    if (state->arg_num < 1)
      argp_usage(state);
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = {opts, parse_opts, args_doc, doc};

int main(int argc, char **argv) {
  struct arguments arguments;
  struct URL_Components *url_components;
  char *msg;
  int ret;

  // Default values
  arguments.verbose = 1;
  arguments.url_count = 0;
  arguments.data = "";
  arguments.headers = malloc(512);
  arguments.method = "GET";
  arguments.url = NULL;

  arguments.url = (char **)malloc(5 * sizeof(char *));
  if (arguments.url == NULL) {
    fprintf(stderr, "Memory allocation for URLs failed!");
    return 1;
  }

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  // TODO: Accept more than one url
  // if (arguments.url_count > 1) {
  //   for (int i = 0; i < arguments.url_count; i++) {
  //     printf("%s\n", arguments.url[i]);
  //   }

  //   return 0;
  // }

  url_components = url_parser(arguments.url[0]);
  upper_string(url_components->scheme);

  msg = generate_request_message(url_components, arguments.method,
                                arguments.headers, arguments.data,
                                strlen(arguments.data));

  printf("%s\n", msg);

 if (strcmp(url_components->scheme, "HTTPS") == 0) {
   ret = make_https_request(url_components, msg);
   if (ret <= 0) {
     printf("Error making request (%d)", ret);
   }
 } else {
   ret = make_http_request(url_components, msg);
   if (ret != 0) {
     perror("Error making request");
   }
 }

  return 0;
}
