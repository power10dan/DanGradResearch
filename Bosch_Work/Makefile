CC = g++
CFLAGS = -lzmq -o 

default: sample

sample:
	$(CC) -g hwclient.cpp $(CFLAGS) hwclien
	$(CC) -g hwserver.cpp $(CFLAGS) hwserve

clean:
	$(RM) hwclien
	$(RM) hwserve

