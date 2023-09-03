#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  char string1[1000];
  char server_message[2000];

  memset(string1, '\0', sizeof(string1));
  memset(server_message, '\0', sizeof(server_message));

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8080);
  if (connect(socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    printf("Connection not successful");
    return -1;
  }

  printf("\nConnection is successful\n");
  scanf("%s", string1);
  printf("Sending message to the server %s", string1);

  if (send(socket_descriptor, string1, sizeof(string1), 0) < 0) {
    printf("\n Cannot send the message\n");
  }

  if (recv(socket_descriptor, server_message, 1024, 0) < 0) {
    printf("\nError while receiving server's msg\n");
    return -1;
  }

  printf("Here is the response %s\n", server_message);

  close(socket_descriptor);
  return 0;
}
