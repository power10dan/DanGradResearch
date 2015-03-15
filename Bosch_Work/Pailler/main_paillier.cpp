#include<zmq.hpp>

#include "Paillier_Client1.h"

int main(){

	Big cipher;
	P_Client1 pclient;
	cipher = pclient.Encrypt("34");
	cipher = pclient.Communicate(cipher);
	pclient.Decrypt(cipher);	
}

