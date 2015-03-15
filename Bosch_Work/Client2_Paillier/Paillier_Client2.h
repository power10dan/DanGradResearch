
#include "big.h"

typedef struct{

	Big public_key;
	Big cipher_txt;

} pub_cipher;

class P_Client2{

	public:
		pub_cipher Encrypt(string);
		void Communicate(Big, Big);
		void Decrypt(Big);
};
