// 0MQ
#include<zmq.h>
#include<unistd.h>
// C includes
#include<stdio.h>
#include<stdlib.h>
// With MIRACL includes
#include "Client1.h"

#define CLIENT_1 1
#define CLIENT_2 2
#define CLIENT_3 3
#define OCTET_SIZE 100
#define MAC_SIZE 20
#define CIPHER_SIZE 100

int main(int argc, char *argv[]){
	
	key_mac mac_ciph_pair;
	
	// init octets in struct
	OCTET_INIT(&(mac_ciph_pair.key), OCTET_SIZE);
	OCTET_INIT(&(mac_ciph_pair.MAC), OCTET_SIZE);
	OCTET_INIT(&(mac_ciph_pair.cipher), OCTET_SIZE);
	OCTET_INIT(&(mac_ciph_pair.key_2), OCTET_SIZE);
	OCTET_INIT(&(mac_ciph_pair.key_3), OCTET_SIZE);
	// generate keys for various clients 
	mac_ciph_pair = key_gen(mac_ciph_pair, 1, CLIENT_1);
	mac_ciph_pair = key_gen(mac_ciph_pair, 4, CLIENT_2);
	mac_ciph_pair = key_gen(mac_ciph_pair, 5, CLIENT_3);
	// encryption ops
	mac_ciph_pair = encrypt("MY Secret", mac_ciph_pair);
	// socket ops
	if(strcmp(argv[1], "2") == 0){
		printf("Sending to client 2\n");
		communicate(mac_ciph_pair.cipher, mac_ciph_pair, CLIENT_2);
	}

	if(strcmp(argv[1], "3")== 0){
		printf("Sending to client 3\n");
		communicate(mac_ciph_pair.cipher, mac_ciph_pair, CLIENT_3);
	}

}

// compute key values for various clients, 
// value represents the value of the key
key_mac key_gen(key_mac mac_ciph, int value, int client){

	octet key_temp;
	OCTET_INIT(&key_temp, OCTET_SIZE);
	int i;
	// randomly generate a key
	key_temp.len = 16;
	for(i = 0; i < 16; i++){ 
		key_temp.val[i] = value;
	}
	
	// identify which client key user wants
	if (client == CLIENT_1){
		OCTET_COPY(&key_temp, &(mac_ciph.key));
	}

	if(client == CLIENT_2){
		OCTET_COPY(&key_temp, &(mac_ciph.key_2));
	}

	if(client == CLIENT_3){
		OCTET_COPY(&key_temp, &(mac_ciph.key_3));
	}

	return mac_ciph;
}

key_mac encrypt(char* message, key_mac mac_ciph){

	octet MAC_tag, cipher, plain_txt;
	int i;
	// init the octet to size 100
	OCTET_INIT(&MAC_tag, OCTET_SIZE);
	OCTET_INIT(&plain_txt, OCTET_SIZE);
	OCTET_INIT(&cipher, OCTET_SIZE);	
	OCTET_JOIN_STRING(message, &plain_txt); 
	// perform AES encryption in Block CBC-IV0 Mode
	BOOL res_encrypt = AES_CBC_IV0_ENCRYPT(&(mac_ciph.key), &plain_txt, NULL, &cipher,NULL);
	// compute mac address for the cipher text	
	BOOL res= MAC1(&cipher, NULL, &(mac_ciph.key), MAC_SIZE, SHA256, &(mac_ciph.MAC));	
	OCTET_COPY(&cipher, &(mac_ciph.cipher));
	printf("Encryption Complete\n");	

	return mac_ciph;
}

void decrypt(octet cipher_txt, key_mac mac_ciph){

	octet plain_txt, new_cipher, mac;

	OCTET_INIT(&new_cipher, OCTET_SIZE);
	OCTET_INIT(&plain_txt, OCTET_SIZE);
    OCTET_INIT(&mac, OCTET_SIZE);
	OCTET_CHOP(&cipher_txt, 20, &new_cipher);

	BOOL res = MAC1(&new_cipher, NULL, &(mac_ciph.key_2), 
					20, SHA256, &mac);

	if(OCTET_COMPARE(&cipher_txt, &(mac))){

		BOOL res_decrypt = AES_CBC_IV0_DECRYPT(&(mac_ciph.key_2), 
							&new_cipher, NULL, &plain_txt,NULL);
		printf("Decryption Success\n");
		printf("My plain text: ");
		OCTET_PRINT_STRING(&plain_txt);
		printf("\n");
	} else {
		printf("Decryption failed\n");
	}

			
}

void communicate(octet MAC_cipher, key_mac mac_ciph_pair, int client){

	octet cipher, recv_cipher;
	OCTET_INIT(&cipher, OCTET_SIZE);
	OCTET_INIT(&recv_cipher, OCTET_SIZE);
	char cipher_val[100];

	void *context = zmq_ctx_new();
	void *requester = zmq_socket (context, ZMQ_REQ);
	// check which client to send to
	if(client == 2){
		printf("Connecting to client 2...\n");
		zmq_connect(requester, "tcp://192.168.191.142:5555");
	} else {
		printf("Connecting to client 3...\n");
		zmq_connect(requester, "tcp://192.168.191.145:5557");

	}

	// concatenate MAC with cipher and send	
	OCTET_JOIN_OCTET(&(MAC_cipher), &(mac_ciph_pair.MAC));	
	// first, send out MAC address
	printf("sending cipher text\n");
	zmq_send(requester, mac_ciph_pair.MAC.val, OCTET_SIZE, 0); 
	printf("receiving cipher\n");
	zmq_recv(requester, cipher_val, OCTET_SIZE,0);
	// join cipher into another octet
	OCTET_JOIN_STRING(cipher_val, &recv_cipher);
	zmq_close(requester);	
	zmq_ctx_destroy(context);
	// decrypt the cipher
	decrypt(recv_cipher, mac_ciph_pair);
	
	printf("Operation Complete\n");
}

