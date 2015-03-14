// MIRACL includes
#include "big.h"

class P_Client1{
	public:
		Big  Encrypt(string);
		Big  Communicate(Big);
		void Decrypt(Big);
};
