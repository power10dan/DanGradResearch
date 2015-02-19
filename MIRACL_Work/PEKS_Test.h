#ifndef __PEKS_Test_H__
#define __PEKS_TEST_H__

#include <string>

#define MR_PAIRING_BN
#define AES_SECURITY 128

#include "pairing_3.h"

#define BUFFER 160

struct Storage{
	// ciphertext for the keyword [PA, PB] 
	G1 PA[BUFFER]; 
	Big PB[BUFFER];
	G2 HW[BUFFER];
	// keyword and text file 
	string *keyword;
	string *text_file;
};

struct Trap{
	G2 TW[BUFFER];

};

class Client{
	public:
		Storage Encryption(PFC&, Big, G1, G1);
};

class Company{
	public:
		G1 Gen_Pub_key(PFC&, Big, G1);
		Big Gen_Priv_Key(PFC&, Big);
		G1 Gen_Generator(PFC&, G1);
		Trap Gen_Trap(PFC&, Big, Storage);
		void Querry_Data(PFC&, Storage, Trap);
};

#endif
