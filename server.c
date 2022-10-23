#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"

int main(int argc, char *argv[]) {
  printf("server booted\n");

  if (argc < 3) {
    printf("parametros nao informados");
    return -1;
  }

  char *ipVersion = argv[1];
  char *port = argv[2];
  printf("ipVersion: %s\n", ipVersion);
  printf("port: %s\n", port);

  return 0;
}