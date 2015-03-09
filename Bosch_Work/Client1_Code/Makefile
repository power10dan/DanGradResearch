CC = gcc
CFLAGS = -lzmq -o 

default: server

server:
	$(CC) -g Client1_Imp.c p1363.c miracl.a $(CFLAGS) shared_secret

test:
	$(CC) -g test.c p1363.c miracl.a $(CFLAGS) test_drive
clean:
	$(RM) shared_secret
	$(RM) test_drive

