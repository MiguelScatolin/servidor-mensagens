all: client server

common.o: common.c common.h
	gcc -c common.c

client: client.c common.o
	gcc -o client client.c common.o

server: server.c common.o
	gcc -o server server.c common.o

clean:
	@rm -rf client server common.o