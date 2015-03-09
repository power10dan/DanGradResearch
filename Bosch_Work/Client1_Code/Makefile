CC = gcc
CFLAGS = -lzmq -o 

default: server

server:
	$(CC) -g Client1_Imp.c p1363.c miracl.a $(CFLAGS) shared_secret

clean:
	$(RM) shared_secret

