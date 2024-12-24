#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <socket/handlers.h>

void init_sockaddr(struct sockaddr_in *name,
                  const char *hostname,
                  uint16_t port)
{
  struct hostent *hostinfo;

  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL) {
    fprintf(stderr, "Unknown host %s.\n", hostname);
    exit(EXIT_FAILURE);
  }
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}

int create_socket() {
  int ret;

  ret = socket(PF_INET, SOCK_STREAM, 0);
  if (ret < 0) {
    printf("Error on creating socket\n");
    exit(EXIT_FAILURE);
  }

  return ret;
}

void parse_data(char *data, size_t data_len) {
  char *headers_end = strstr(data, "\r\n\r\n");
  if (headers_end) {
    size_t headers_len = headers_end - data;
    size_t body = headers_len + 4;

    // printf("Headers:\n%.*s\n\n", (int)headers_len, data);
    printf("%s\n", data + body);
    
  } else {
    printf("Invalid or incomplete response\n");
  }
}

void read_from_socket(int sock) {
  char buffer[4096];
  char *data = NULL;
  int total_bytes = 0;
  int bytes_received;

  memset(buffer, 0, sizeof(buffer));

  while ((bytes_received = recv(sock, buffer + total_bytes, 
          sizeof(buffer) - total_bytes, 0)) > 0) {
    data = realloc(data, total_bytes + bytes_received + 1);
    if (!data) {
      perror("Memory allocation failed");
      return;
    }

    memcpy(data + total_bytes, buffer, bytes_received);
    total_bytes += bytes_received;
    data[total_bytes] = '\0';
  }

  if (total_bytes) {
    parse_data(data, total_bytes);
  } else {
    printf("No data received");
  }

  free(data);
}

void read_from_BIO_socket(SSL *ssl) {
  char buffer[4096];
  char *data = NULL;
  size_t nbytes;
  int total_bytes = 0;


  while (SSL_read_ex(ssl, buffer, sizeof(buffer), &nbytes)) {
    data = realloc(data, total_bytes + nbytes + 1);
    if (!data) {
      perror("Memory allocation failed");
      return;
    }

    memcpy(data + total_bytes, buffer, nbytes);
    total_bytes += nbytes;
    data[total_bytes] = '\0';
  }

  if (total_bytes) {
    parse_data(data, total_bytes);
  } else {
    printf("No data received");
  }

  free(data);
}

BIO *create_bio_socket(char *hostname, char *port) {
  BIO *bio;
  BIO_ADDRINFO *res;
  int sock = -1;
  const BIO_ADDRINFO *ai = NULL;

  if (!port)
    port = "443";

  if (!BIO_lookup_ex(hostname, port, BIO_LOOKUP_CLIENT, PF_INET, SOCK_STREAM, 0, &res))
    return NULL;

  for (ai = res; ai != NULL; ai = BIO_ADDRINFO_next(ai)) {
    sock = BIO_socket(BIO_ADDRINFO_family(ai), SOCK_STREAM, 0, 0);
    if (sock == -1) {
      continue;
    }

    if (!BIO_connect(sock, BIO_ADDRINFO_address(ai), BIO_SOCK_NODELAY)) {
      BIO_closesocket(sock);
      sock = -1;
      continue;
    }

    break;
  }

  BIO_ADDRINFO_free(res);

  bio = BIO_new(BIO_s_socket());
  if (bio == NULL) {
    BIO_closesocket(sock);
    return NULL;
  }

  BIO_set_fd(bio, sock, BIO_CLOSE);

  return bio;
}
