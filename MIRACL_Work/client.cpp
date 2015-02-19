#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>

#include "PEKS_Test.h"

int main(){

	// init class
	PFC pfc(AES_SECURITY);
	Company comp;
	Client client;
	Trap trap;
	Storage storage;	
	// encryption variables 
	G1 g, h, PA;
	Big alpha, r, PB;
	GT t;
	// generate generator
	g = comp.Gen_Generator(pfc, g);
//	pfc.precomp_for_mult(g);
	// generate private key
	alpha = comp.Gen_Priv_Key(pfc, alpha); 
	// generate public key 
	h = comp.Gen_Pub_key(pfc, alpha, g);
//	pfc.precomp_for_mult(h);
	// client takes the generated key and generator 
	// and encrypt his or her file 
	storage = client.Encryption(pfc, r, h, g);
	trap = comp.Gen_Trap(pfc, alpha, storage);
 	// decryption step 
	comp.Querry_Data(pfc, storage, trap);
}
