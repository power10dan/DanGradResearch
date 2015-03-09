// 0MQ includes
#include<zmq.h>
// C includes
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
// usual client includes 
#include "Client2.h"

#define CLIENT_1 1
#define CLIENT_2 2
#define CLIENT_3 3
#define MAC_SIZE 20
#define OCTET_SIZE 100
#define CIPHER_SIZE 100

int main(){

	key_mac mac_ciph_pair;
	OCTET_INIT(&(mac_ciph_pair.key), MAC_SIZE);
	OCTET_INIT(&(mac_ciph_pair.MAC), MAC_SIZE);
	OCTET_INIT(&(mac_ciph_pair.cipher), MAC_SIZE);
	OCTET_INIT(&(mac_ciph_pair.key_1), MAC_SIZE);
	OCTET_INIT(&(mac_ciph_pair.key_3), MAC_SIZE);

	mac_ciph_pair = key_gen(mac_ciph_pair, 1, CLIENT_1);
	mac_ciph_pair = key_gen(mac_ciph_pair, 4, CLIENT_2);
	mac_ciph_pair = key_gen(mac_ciph_pair, 5, CLIENT_3);
	mac_ciph_pair = encrypt("response", mac_ciph_pair);
	communicate(mac_ciph_pair);

}

void communicate(key_mac mac_ciph){
	octet cipher;
	char val[CIPHER_SIZE];
	int i;

	OCTET_INIT(&cipher, CIPHER_SIZE);
	
	// socket inits
	void *context = zmq_ctx_new();
	void *responder = zmq_socket(context, ZMQ_REP);
	zmq_bind(responder, "tcp://*:5555");

	// begin socket receive opts
	printf("Server waiting\n");
	zmq_recv(responder, val, OCTET_SIZE, 0);
	printf("Message Received\n");
	zmq_send(responder, mac_ciph.cipher.val, OCTET_SIZE, 0);
	printf("response message sent\n");
	OCTET_JOIN_STRING(val, &cipher);
	zmq_close(responder);
	zmq_ctx_destroy(context);
	decrypt(cipher, mac_ciph);
}

// Compute various client's keys
key_mac key_gen(key_mac MAC_ciph, int value, int client){	

	octet key;
	int i;

	OCTET_INIT(&key, OCTET_SIZE);
	key.len = 16;	

	for(i = 0; i < 16; i++){
		key.val[i] = value;		
	}

	if(client == CLIENT_1){
		OCTET_COPY(&key, &(MAC_ciph.key_1));
	}

	if(client == CLIENT_2){
		OCTET_COPY(&key, &(MAC_ciph.key));
	}

	if(client == CLIENT_3){
		OCTET_COPY(&key, &(MAC_ciph.key_3));
	}

	return MAC_ciph;
}

void decrypt(octet cipher, key_mac mac_cipher_p){
	octet plain_txt;

	OCTET_INIT(&plain_txt, OCTET_SIZE);	

	BOOL res_dec = AES_CBC_IV0_DECRYPT(&(mac_cipher_p.key_1), 
										&cipher, NULL, &plain_txt,
											NULL);
	if(res_dec == 1){
		printf("Decryption Success\n");
	}

	if(res_dec == 0){
		BOOL res_dec2 = AES_CBC_IV0_DECRYPT(&(mac_cipher_p.key_3), &cipher,
									NULL, &plain_txt, NULL);
	}

	printf("My Plain text is: ");
	OCTET_PRINT_STRING(&plain_txt);
	printf("\n");

}


key_mac encrypt(char* message, key_mac key_ciph){

	octet MAC_tag, cipher, plain_txt;

	OCTET_INIT(&MAC_tag, OCTET_SIZE);
	OCTET_INIT(&cipher, OCTET_SIZE);
	OCTET_INIT(&plain_txt, OCTET_SIZE);
	OCTET_JOIN_STRING(message, &plain_txt);

	BOOL res_encrypt = AES_CBC_IV0_ENCRYPT(&(key_ciph.key),&plain_txt, 
								NULL, &cipher, NULL); 
	
	BOOL res = MAC1(&cipher, NULL, &(key_ciph.key), MAC_SIZE, 
							SHA256, &(key_ciph.MAC));
	OCTET_COPY(&cipher, &(key_ciph.cipher));
	printf("Encryption Complete\n");

	return key_ciph;
}
