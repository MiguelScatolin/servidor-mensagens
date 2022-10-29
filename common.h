#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>

#define BUFSZ 1024

void sendMessage();
char *receiveMessage(int s);
char *addressToString(const struct sockaddr *address);
struct sockaddr_storage parseAddress(char *ipAdress, char *port);
void logexit(char *errorMessage);
bool stringEqual(char *s1, char *s2);