// 0MQ
#include<zmq.h>
#include<unistd.h>
// C includes
#include<stdio.h>
#include<stdlib.h>
// With MIRACL includes
#include "Client1.h"

int main(){
	key_mac mac_ciph_pair;
	mac_ciph_pair = encrypt("TestMessage", mac_ciph_pair);
	printf("Cipher Text To Be Sent\n");
	OCTET_OUTPUT(&(mac_ciph_pair.cipher));
	communicate(mac_ciph_pair.cipher);
}

key_mac encrypt(char* message, key_mac mac_ciph){

	octet MAC_tag, cipher, plain_txt, key_temp;
	int i;
	OCTET_INIT(&MAC_tag, 100);
	OCTET_INIT(&key_temp, 100);
	OCTET_INIT(&plain_txt, 100);
	OCTET_INIT(&cipher, 100);	
	OCTET_INIT(&(mac_ciph.key), 100);
	OCTET_INIT(&(mac_ciph.MAC), 100);
	OCTET_INIT(&(mac_ciph.cipher), 100);
	OCTET_JOIN_STRING(message, &plain_txt); 
	// randomly generate a key
	key_temp.len = 16;
	for(i = 0; i < 16; i++){ 
		key_temp.val[i] = 1;
	}
	OCTET_COPY(&key_temp, &(mac_ciph.key));
	// perform AES encryption in Block CBC-IV0 Mode
	BOOL res_encrypt = AES_CBC_IV0_ENCRYPT(&key_temp, &plain_txt, NULL, &cipher,NULL);
	// compute mac address for the cipher text
	BOOL res= MAC1(&cipher, NULL, &key_temp, 20, SHA256, &MAC_tag);	
	// save MAC address
	OCTET_COPY(&MAC_tag, &(mac_ciph.MAC));
	OCTET_JOIN_OCTET(&cipher, &MAC_tag);
	OCTET_COPY(&MAC_tag, &(mac_ciph.cipher));
	printf("Encryption Complete\n");	

	return mac_ciph;
}

/*void decrypt(octet cipher_txt, octet* key, octet *mac){

	octet mac2, plain_txt;
	OCTET_INIT(&mac2, 100);
	OCTET_INIT(&plain_txt, 100); 
	key_mac mac_ciph;
	
	// compute  MAC with the received cipher text and shared key
	BOOL res = MAC1(&cipher_txt, NULL, &mac_ciph.key, 20, SHA256, &mac2);
	// if MAC matches, then decrypt the message and print out plain text
	if(OCTET_COMPARE(&mac, &mac2)){
		BOOL res_decrypt = AES_CBC_IV0_DECRYPT(&key, &cipher_txt,
								NULL, &plain_txt,NULL);
		printf("My plain text: ");
		OCTET_OUTPUT_STRING(&plain_txt);
	}else{
		printf("Wrong MAC\n");
	}

}*/

void communicate(octet MAC_cipher){
	void *context = zmq_ctx_new();
	void *requester = zmq_socket (context, ZMQ_REQ);
	zmq_connect(requester, "tcp://192.168.38.140:5555");

	zmq_send(requester, MAC_cipher.val, 100,0); 
	printf("Cipher text sending...\n");
	zmq_close(requester);
	zmq_ctx_destroy(context);
	printf("Cipher text sent\n");
}

