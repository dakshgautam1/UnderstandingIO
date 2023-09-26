#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>  // for close

int main(int argc, char *argv[]) {
  char client_message[2000];
  socklen_t client_size;
  struct sockaddr_in server_address;

  // clear buffers
  strcpy(client_message, "");

  int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);

  bind(socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address));

  listen(socket_descriptor, 100);  // 100 is the backlog number, meaning upto 100 connections can be kept int he queue.

  printf("\nHere is the socket descriptor: %d", socket_descriptor);

  while (1) {
    struct sockaddr_in client_address;
    memset(&client_address, '\0', sizeof(client_address));
    printf("\nListening for incoming connections.....\n");
    client_size = sizeof(client_address);

    int client_descriptor = accept(socket_descriptor, (struct sockaddr *)&client_address, &client_size);
    printf("PORT: %d\n", ntohs(client_address.sin_port));

    if (recv(client_descriptor, client_message, sizeof(client_message), 0) < 0) {
      printf("Could not be received");
      return -1;
    }

    printf("\nhere is the message: %s \n", client_message);

    char server_message[10000];
    memset(server_message, '\0', sizeof(server_message));

    strcat(server_message, "SERVER: ");
    strcat(server_message, client_message);

    send(client_descriptor, server_message, sizeof(server_message), 0);

    if (client_descriptor == -1) {
      exit(1);
    }
    close(client_descriptor);
  }
}