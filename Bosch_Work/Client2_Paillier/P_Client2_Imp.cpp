#include<zmq.hpp>

#include<iostream>
#include<fstream>

#include"Paillier_Client2.h"
#include "crt.h"

Miracl precision(100,0);

pub_cipher P_Client2::Encrypt(string message){

	Big cipher, generator, random, message_Big, pub_key, pub_key_sqr, d;
	pub_cipher pciph;
	// read from pre-generated public key
	ifstream public_key("public.key");
	// variable inits
	miracl *mip = &precision;
	long seed = 989989989;
	char t[100];	
	mip->IOBASE = 16;
	public_key >> pub_key;
	pub_key_sqr = pub_key*pub_key;

	random = pow(rand(pub_key_sqr), pub_key, pub_key_sqr);
	
	strcpy(t, message.c_str());
	message_Big = from_binary(strlen(t), t);

	generator = 1+message_Big*pub_key;
	cipher = modmult(generator, random, pub_key_sqr);

	std::cout << "My message " << endl;
	std::cout << message_Big << endl;

	pciph.public_key = pub_key;
	pciph.cipher_txt = cipher;
	
	return pciph;
}

Big CRT(Big *p, Big *r){

	Crt chinese(2,p);
	return chinese.eval(r);

}

void P_Client2::Decrypt(Big cipher){

	Big lcm,  message, d, n, n2, p, q, p2, q2, er[2], em[2];

	ifstream private_key("private.key");	
	miracl *mip = &precision;
	int len;
	char t[800];
	mip->IOBASE = 16;
	private_key >> p >> q;
	n=p*q;
	n2 = n*n;
	p2=p*p;
	q2 = q*q;
	lcm = (((p-1)*(q-1))/ gcd(p-1, q-1));

	em[0] = lcm;
	em[1] = n;
	er[0] = 0;
	er[1] = 1;
	d=CRT(em,er);

	em[0] = p2;
	em[1] = q2;
	er[0] = pow(cipher, d%(p2-p), p2);
   	er[1] = pow(cipher, d%(q2-q), q2);
	message = CRT(em, er);

	message = (message-1) / n;
	cout << "Your message " << endl;
	cout << message << endl;

}

void  P_Client2:: Communicate(Big cipher, Big pub_key){

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	
	pub_cipher p_cipher;
	
	std::cout << "Waiting for package to arrive..." << std::endl;
	socket.bind("tcp://*:5555");

	zmq::message_t request(800);
	// turn BIG to char for socket sending
	char cipher_big_to_char[800], cipher_recv[800];
	Big recv_cipher;
	cipher_big_to_char << cipher;
	cipher_big_to_char[800] = 0;
	memcpy((void*) request.data(), cipher_big_to_char, 800);
	
	// wait for reply
	zmq::message_t reply(800);
	socket.recv(&reply);
	std::cout << "Received cipher text" << std::endl;

	memcpy(cipher_recv, (void *) reply.data(), 800);
	recv_cipher = cipher_recv;
	// compute sum of cipher text to get sum of plain txt when decrypt
	
	Big ciph2 = pub_key * pub_key;
	recv_cipher = modmult(recv_cipher, cipher,ciph2); 
	
	// package for sending
	char respond[800];
	respond << recv_cipher;
	respond[800] = 0;
	memcpy((void *) reply.data(), respond, 800);
	// Send the cipher txt
	std::cout << "Connected to server, sending cipher" << std::endl;
	socket.send(reply);
	std::cout << "Cipher sent" << std::endl;
}

