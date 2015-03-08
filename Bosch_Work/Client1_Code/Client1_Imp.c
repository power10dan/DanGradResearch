// 0MQ
#include<zmq.h>
#include<stdio.h>
#include<stdlib.h>
// With MIRACL includes
#include "Client1.h"

int main(){

	




}

octet encrypt(char* message, octet* key){

	octet MAC_tag, cipher, plain_txt;
	key_mac  mac_ciph;

	OCTET_INIT(&MAC_tag, 100);
	OCTET_INIT(&cipher, 100);
	OCTET_INIT(&plain_txt, 100);
	OCTET_JOIN_STRING(message, &plain_txt); 

	// perform AES encryption in Block CBC-IV0 Mode
	BOOL res_encrypt = AES_CBC_IV0_ENCRYPT(&key, &plain_txt, 
								NULL, &cipher,NULL);
	// compute mac address for the cipher text
	BOOL res= MAC1(&cipher, NULL, &key, 20, SHA256, &MAC_tag);
	printf("Encrypted word\n");
	OCTET_JOIN_OCTET(&cipher, &MAC_tag);	

	return MAC_tag;
}

void decrypt(octet* cipher_txt, octet* key, octet *mac){

	octet mac2, plain_txt;
	OCTET_INIT(&mac2, 100);
	OCTET_INIT(&plain_txt, 100); 

	// compute  MAC with the received cipher text and shared key
	BOOL res = MAC1(&cipher_txt, NULL, &key, 20, SHA256, &mac2);

	// if MAC matches, then decrypt the message and print out plain text
	if(OCTET_COMPARE(&mac, &mac2)){

		BOOL res_decrypt = AES_CBC_IV0_DECRYPT(&key, &cipher_txt,
								NULL, &plain_txt,NULL);
		printf("My plain text: ");
		OCTET_OUTPUT_STRING(&plain_txt);

	} else{
		printf("Wrong MAC\n");
	}

}

void communicate(octet* MAC_cipher){

	// set up client to send package
	
	



}

