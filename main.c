#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <helpers/helpers.h>
#include <HTTP/http.h>

static int verbose_flag;
static int post_flag;

void usage(void) {
  printf("petito [options] <url>\n"
      "-h, --help       Print this message\n"
      "-d, --data       HTTP POST data\n"
      "-H, --header     Add header to request\n"
      "-u, --url        Specify url (optional)\n"
      "-x, --post       Specify request as a POST message(optional)\n"
      "-v, --verbose    Show more information on request\n"
  );
}

int main(int argc, char **argv) {
  int ret, opt;
  char *message = NULL;
  char *data = NULL;
  char *headers = malloc(512);
  struct URL_Components* components;

  static struct option long_options[] =
    {
      {"verbose", no_argument, &verbose_flag, 1},
      {"verbose", no_argument, &verbose_flag, 0},
      {"post", no_argument, &post_flag, 1},
      {"post", no_argument, &post_flag, 0},
      {"help", no_argument, NULL, 'h'},
      {"header", required_argument, NULL, 'H'},
      {"data", required_argument, NULL, 'd'},
      {"url", required_argument, NULL, 'u'},
      {0, 0, 0, 0}
    };

  if (argc < 2) {
    usage();
    return 0;
  }

  while (1) {
    int opt_index = 0;
    opt = getopt_long(argc, argv, "d:H:u:xv", long_options, &opt_index);

    if (opt == -1)
      break;

    switch (opt) {
      case 0:
        if (long_options[opt_index].flag != 0)
          break;
        printf ("option %s", long_options[opt_index].name);
        if (optarg)
          printf (" with arg %s", optarg);
        printf ("\n");
        break;

      case 'h':
        usage();
        return 0;        

      case 'H':
        strcat(headers, optarg);
        strcat(headers, "\r\n");
        break;

      case 'd':
        post_flag = 1;
        data = strdup(optarg);
        break;

      case 'u':
        components = url_parser(optarg);
        break;

      case 'x':
        post_flag = 1;
        break;

      case '?':
        puts("not known argument");
        break;

      default:
        abort();
    }
  }

  if (verbose_flag)
    puts("verbose set");

  if (optind < argc) {
    while (optind < argc) {
      components = url_parser(argv[optind++]);
    }
  }

  if (post_flag) {
    message = generate_post_request_message(components, headers, data, strlen(data));
  } else {
    message = generate_get_request_message(components);
  }

  if (strcmp(components->scheme, "https") == 0) {
    ret = make_https_request(components, message);
    if (ret <= 0) {
      printf("Error making request (%d)", ret);
    }
  } else {
    ret = make_http_request(components, message);
    if (ret != 0) {
      perror("Error making request");
    }
  }

  return 0;
}
