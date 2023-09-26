#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main() {
  printf("Configuring local address\n");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;
  getaddrinfo(0, "8080", &hints, &bind_address);

  printf("Creating socket...\n");
  int server_descriptor;
  server_descriptor = socket(
      bind_address->ai_family,
      bind_address->ai_socktype,
      bind_address->ai_protocol);

  // make the socket non-blocking

  if (server_descriptor < 0) {
    fprintf(stderr, "socket() failed");
    return -1;
  }

  printf("Binding the scoket to port \n");
  if (bind(server_descriptor, bind_address->ai_addr, bind_address->ai_addrlen)) {
    fprintf(stderr, "bind() failed");
    return -1;
  }

  free(bind_address);

  printf("Starting to list to connections\n");
  if (listen(server_descriptor, 10) < 0) {
    fprintf(stderr, "listen() failure");
    return -1;
  }

  while (1) {
    printf("Waiting for the connections\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    int client_descriptor = accept(
        server_descriptor,
        (struct sockaddr *)&client_address,
        &client_len);

    if (client_descriptor < 0) {
      fprintf(stderr, "accept() failed");
      return -1;
    }

    printf("Client is connected\n");
    char address_buffer[1000];

    getnameinfo(
        (struct sockaddr *)&client_address,
        client_len,
        address_buffer,
        sizeof(address_buffer),
        0,
        0,
        NI_NUMERICHOST);

    printf("%s\n", address_buffer);

    printf("Reading the request...\n");

    // clearing the array
    char request[1024];
    memset(request, '\0', sizeof(request));

    // Fetching the bytes from the client.
    int bytes_received = recv(client_descriptor, request, 1024, 0);

    printf("Recieved %d bytes.\n", bytes_received);
    printf("Output : %s\n", request);

    printf("Sending response...\n");

    // Resetting the array.
    char server_response[1024];
    memset(server_response, '\0', sizeof(server_response));
    strcat(server_response, " SERVER: ");
    strcat(server_response, request);
    int bytes_sent = send(client_descriptor, server_response, strlen(server_response), 0);
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(server_response));

    printf("Closing the connection\n");
    close(client_descriptor);
  }

  close(server_descriptor);

  return 0;
}
