all: client server

common.o: common.c common.h
	gcc -Wall -c common.c

client: client.c common.o
	gcc -Wall -o client client.c common.o

server: server.c common.o
	gcc -Wall -o server server.c common.o

clean:
	@rm -rf client server common.o