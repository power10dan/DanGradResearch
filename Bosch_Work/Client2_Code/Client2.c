// 0MQ includes
#include<zmq.h>
// C includes
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include "Client2.h"

int main(){

	communicate();

}

void communicate(){

	void *context = zmq_ctx_new();
	void *responder = zmq_socket(context, ZMQ_REP);
	zmq_bind(responder, "tcp://192.168.38.140:5555");
	
	char val[100], w;
	int i;
	printf("Server waiting\n");
	zmq_recv(responder, val, 100, 0);
	printf("Message Received\n");

	for(i = 0; i < 100; i++){
		w = val[i];
		printf("%02x", w);
	}
}

