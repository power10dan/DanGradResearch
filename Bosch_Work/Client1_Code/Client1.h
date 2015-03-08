#ifndef CLIENT1_H_
#define CLIENT1_H_

#include "p1363.h"

typedef struct{	
	
	octet key;
	octet MAC;
	octet cipher;
	// pre-shared keys
	octet key_2;
	octet key_3;

} key_mac;

// client function prototypes
void communicate(octet);
key_mac encrypt(char*, key_mac);
void decrypt(octet*, octet*, octet*);

#endif
