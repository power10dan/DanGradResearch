CC = gcc
CC2 = g++
CFLAGS = -lzmq -o 

default: server

server:
	$(CC) -g Client2.c p1363.c miracl.a $(CFLAGS) Client2
paillier:
	$(CC2) main_Pailler.cpp P_Client2_Imp.cpp big.cpp crt.cpp miracl.a $(CFLAGS) pai_2

clean:
	$(RM) Client2
	$(RM) pai_2

