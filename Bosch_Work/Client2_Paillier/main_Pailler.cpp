#include<zmq.hpp>
#include<string>
#include "Paillier_Client2.h"

int main(){

	pub_cipher p_ciph;
	P_Client2 p_client2;
	// encrypt another cipher for addition
	p_ciph = p_client2.Encrypt("36");
	p_client2.Communicate(p_ciph.cipher_txt, p_ciph.public_key);

}
