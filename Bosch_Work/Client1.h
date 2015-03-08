#ifndef CLIENT1_H_
#define CLIENT1_H_

#include "p1363.h"

typedef struct{

	octet key;
	octet MAC;
} key_mac;

// client function prototypes
void communicate(octet*);
void receive(octet*);
octet encrypt(char*, octet*);
void decrypt(octet*, octet*, octet*);

#endif
