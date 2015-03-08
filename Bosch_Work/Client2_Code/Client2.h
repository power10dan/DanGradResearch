#ifndef Client2_H_
#define Client2_H_

#include "p1363.h"

typedef struct{

	octet key; // client 2's unique key
	octet MAC;
	octet cipher;
	
	// pre-shared keys for various clients
	octet key_1;
	octet key_3;

} key_mac;

// server ops 
void communicate();
octet encrypt();
void decrypt();

#endif
