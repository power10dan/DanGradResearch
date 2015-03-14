CC = gcc
CC2 = g++
CFLAGS = -lzmq -o 

default: server

server:
	$(CC) -g Client2.c p1363.c miracl.a $(CFLAGS) Client2
paillier:
	$(CC2) main_paillier.cpp P_Client1_Imp.cpp big.cpp crt.cpp miracl.a $(CFLAGS) pai_1

clean:
	$(RM) Client2

