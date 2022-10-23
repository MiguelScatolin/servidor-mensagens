#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

int main(int argc, char *argv[]) {
  printf("client booted\n");
  if (argc < 3) {
    printf("parametros nao informados");
    return -1;
  }
  char *ipAdress = argv[1];
  char *port = argv[2];
  printf("ipAdress: %s\n", ipAdress);
  printf("port: %s\n", port);

  sendMessage();
  return 0;
}