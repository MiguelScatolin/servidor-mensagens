#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUMERO_MAX_CONEXOES_PENDENTES 10

typedef enum {
  instalar,
  desinstalar,
  ler,
  listar
} operacao;

typedef struct cmd {
  operacao type;
  int switches[3];
  int rack;
} cmd;

operacao getType(char *typeString) {
  if(stringEqual(typeString, "add"))
    return instalar;
  else if(stringEqual(typeString, "rm"))
    return desinstalar;
  else if(stringEqual(typeString, "get"))
    return ler;
  else if(stringEqual(typeString, "ls"))
    return listar;
  else 
    logexit("invalid command");
}

cmd parsecmd(char *s)
{
  char *es;
  cmd comando;

  int value;
  char *val = strtok(s, " ");
  comando.type = getType(val);
  for(int i = 0; i < 3; i++)
    comando.switches[i] = -1;

  switch (comando.type) 
  {
    case instalar:
      val = strtok(NULL, " "); 
      if(!stringEqual(val, "sw"))
        logexit("formato incorreto");

      for(int i = 0; i < 3; i++) {
        val = strtok(NULL, " ");
        if(val == NULL)
          logexit("comando incompleto");
        
        if(stringEqual(val, "in"))
          break;

        if(sscanf(val, "%d", &value) != 1)
          logexit("formato incorreto");
        
        comando.switches[i] = value;
      }

      val = strtok(NULL, " ");
      if(!stringEqual(val, "in") && sscanf(val, "%d", &value) != 1)
        logexit("esperava numero de rack");

      comando.rack = value;
      break;
    case desinstalar:
      
      val = strtok(NULL, " "); 
      if(!stringEqual(val, "sw"))
        logexit("formato incorreto");

      val = strtok(NULL, " ");
      if(val == NULL)
        logexit("comando incompleto");
        
      if(sscanf(val, "%d", &value) != 1)
        logexit("formato incorreto");
      
      comando.switches[0] = value;

      val = strtok(NULL, " ");
      if(val == NULL || !stringEqual(val, "in"))
        logexit("comando incompleto");
        
      val = strtok(NULL, " ");
      if(sscanf(val, "%d", &value) != 1)
        logexit("esperava numero de rack");

      comando.rack = value;
      break;
    case listar:
      val = strtok(NULL, " ");
      if(sscanf(val, "%d", &value) != 1)
        logexit("esperava numero de rack");

      comando.rack = value;
      break;
    case ler:
      for(int i = 0; i < 2; i++) {
        val = strtok(NULL, " ");
        if(val == NULL)
          logexit("comando incompleto");
        
        if(stringEqual(val, "in"))
          break;

        if(sscanf(val, "%d", &value) != 1)
          logexit("formato incorreto");
        
        comando.switches[i] = value;
      }

      val = strtok(NULL, " ");
      if(!stringEqual(val, "in") && sscanf(val, "%d", &value) != 1)
        logexit("esperava numero de rack");

      comando.rack = value;
      break;
    
    default:
      logexit("tipo de comando desconhecido");
  }

  return comando;
}

int runcmd(struct cmd cmd)
{ 
  switch (cmd.type) 
  {
    case instalar:
      //instalarSwitch();
      return 0;
    case desinstalar:
      //desinstalarSwitch();
      return 0;
    case listar:
      //listarSwitches();
      return 0;
    case ler:
      //lerRack();
      return 0;
    
    default:
      logexit("tipo de comando desconhecido");
  }
}

struct sockaddr_storage initializeServerSocket(char *version, char *portString) {
  if(portString == NULL)
    logexit("port nulo");

  uint16_t port = (uint16_t)atoi(portString);
  if(port == 0)
    logexit("port invalida");
  port = htons(port); //host to network short

  struct sockaddr_storage storage;
  if (strcmp(version, "v4") == 0) {
    struct sockaddr_in *addr4 = (struct sockaddr_in *)(&storage);
    addr4->sin_family = AF_INET;
    addr4->sin_addr.s_addr = INADDR_ANY;
    addr4->sin_port = port;
    return storage;
  } else if(strcmp(version, "v6") == 0) {
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)(&storage);
    addr6->sin6_family = AF_INET6;
    addr6->sin6_addr = in6addr_any;
    addr6->sin6_port = port;
    return storage;
  } else
    logexit("nao foi possivel conectar"); 
}

int main(int argc, char *argv[]) {
  if (argc < 3) 
    logexit("parametros nao informados");

  printf("server booted\n");

  char *ipVersion = argv[1];
  char *port = argv[2];
  printf("ipVersion: %s\n", ipVersion);
  printf("port: %s\n", port);

  struct sockaddr_storage storage = initializeServerSocket(ipVersion, port);
  struct sockaddr *address = (struct sockaddr *)(&storage);

  int s = socket(storage.ss_family, SOCK_STREAM, 0);
  if(s == -1)
    logexit("socket");

  if(bind(s, address, sizeof(storage)))
    logexit("bind");

  if(listen(s, NUMERO_MAX_CONEXOES_PENDENTES))
    logexit("bind");

  printf("bound to %s, waiting connections\n", addressToString(address));

  struct sockaddr_storage clientStorage = initializeServerSocket(ipVersion, port);
  struct sockaddr *clientAddress = (struct sockaddr *)(&clientStorage);
  int clientSocket = accept(s, clientAddress, sizeof(clientStorage));
  if(clientSocket == -1)
    logexit("accept");

  char *buf;
  while(1) {
    buf = receiveMessage(s);
    runcmd(parsecmd(buf));
  }

  close(clientSocket);

  exit(EXIT_SUCCESS);
}