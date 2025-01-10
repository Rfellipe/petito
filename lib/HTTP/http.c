#include <HTTP/http.h>
#include <openssl/prov_ssl.h>
#include <openssl/ssl.h>
#include <openssl/tls1.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <socket/handlers.h>
#include <helpers/helpers.h>
#include <openssl/types.h>
#include <openssl/bio.h>

char *generate_request_message(
  struct URL_Components *url_components,
  char *method,
  char *headers,
  char *data,
  size_t data_len
) {
  char *full_path = get_full_path(url_components);
  if (full_path == NULL) 
    full_path = "";

  char *host_with_port = get_host_with_port(url_components);
  char *format = NULL;
  char *buffer = malloc(MAX_MSG);
  if (!buffer) 
    return NULL;

  if ((strcmp(method, "GET") == 0) || 
      strcmp(method, "HEAD") == 0)
  {
    format = ("%s /%s HTTP/1.1\r\n"
      "User-Agent: petito/%s\r\n"
      "Host: %s\r\n"
      "Accept: */*\r\n"
      "Connection: close\r\n"
      "\r\n"
    );

    sprintf(buffer, format, method, full_path, "1.0", host_with_port);

    
    return buffer;
  }

  format = ("%s /%s HTTP/1.1\r\n"
    "User-Agent: petito/%s\r\n"
    "Host: %s\r\n"
    "Accept: */*\r\n"
    "Content-Length: %d\r\n"
    "Connection: close\r\n"
    "%s"
    "\r\n"
    "%s"
  );

  sprintf(buffer, format, method, full_path, "1.0", host_with_port, data_len, headers, data);

  return buffer;
}

int make_http_request(struct URL_Components* components, char *message) {
  int sock, port, nbytes;
  socklen_t size;
  struct sockaddr_in servername;

  if (components->port) {
    port = strtol(components->port, NULL, 10);
  } else {
    port = 80;
  }

  sock = create_socket();
  init_sockaddr(&servername, components->host, port);
  if (connect(sock, (struct sockaddr *)&servername, sizeof(servername)) < 0) {
    perror("Client connection error");
    exit(1);
  }

  free_url_components(components);

  nbytes = write(sock, message, strlen(message) + 1);
  if (nbytes < 0) {
    perror("Error writing message to socket");
    exit(EXIT_FAILURE);
  }

  read_from_socket(sock);

  return 0;
}

int make_https_request(struct URL_Components* components, char *message) {
  char buf[4096];
  size_t written;
  size_t nbytes;
  int total_bytes = 0;

  SSL_CTX *ctx;
  SSL *ssl;
  BIO *bio;

  // Creates a new SSL context to use SSL objects
  ctx = SSL_CTX_new(TLS_client_method());
  if (ctx == NULL) {
    printf("failed to create the SSL_CTX\n");
    return -1;
  }

  // Verify the handshake if the verification fails
  // it aborts the handshake
  SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

  if (!SSL_CTX_set_default_verify_paths(ctx)) {
    printf("Failed to set the default trusted certificate store\n");
    return -2;
  }

  // Set minimal version of TLS to TLS_1.2
  if (!SSL_CTX_set_min_proto_version(ctx, TLS1_1_VERSION)) {
    printf("Failed to set the minimum TLS protocol version\n");
    return -3;
  }

  // Create an SSL object to represent the TLS connection
  ssl = SSL_new(ctx);
  if (ssl == NULL) {
    printf("Failed to create SSL obj");
    return -4;
  }

  bio = create_bio_socket(components->host, components->port);
  if (bio == NULL) {
    perror("Error creationg BIO Socket");
    return -5;
  }

  SSL_set_bio(ssl, bio, bio);

  if (!SSL_set_tlsext_host_name(ssl, components->host)) {
    printf("Failed to set the SNI hostname\n");
    return -6;
  }

  if (!SSL_set1_host(ssl, components->host)) {
    printf("Failed to set the certificate verification hostname");
    return -7;
  }

  int ret = SSL_connect(ssl);

  if (ret < 1) {
    printf("Failed to connect to the server (%d)\n", ret);

    int err = SSL_get_error(ssl, ret);

    return err;
  }

  if (!SSL_write_ex(ssl, message, strlen(message), &written)) {
    printf("Failed to write message\n");
    return -6;
  }

  read_from_BIO_socket(ssl);

  return 1;
}
