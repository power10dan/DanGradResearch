#ifndef Client2_H_
#define Client2_H_

#include "p1363.h"

typedef struct{

	octet cipher;	
	// unique key to current client
	octet key; 
	// shared keys
	octet key_1;
	octet key_3;
	// mac address of each client
	octet MAC;

} key_mac;

void communicate(key_mac);
key_mac key_gen(key_mac, int, int);
key_mac encrypt(char*, key_mac);
void decrypt(octet, key_mac);

#endif
