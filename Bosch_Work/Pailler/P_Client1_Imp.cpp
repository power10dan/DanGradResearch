#include<zmq.hpp>

#include<iostream>
#include<fstream>

#include"Paillier_Client1.h"
#include "crt.h"

Miracl precision(100,0);

Big P_Client1::Encrypt(string message){

	Big cipher, generator, random, message_Big, pub_key, pub_key_sqr, d;

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

	return cipher;
}

Big CRT(Big *p, Big *r){

	Crt chinese(2,p);
	return chinese.eval(r);

}

void P_Client1::Decrypt(Big cipher){

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

Big P_Client1:: Communicate(Big cipher){

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REQ);
	
	std::cout << "Connecting to server, please wait..." << std::endl;
	socket.connect("tcp://192.168.191.142:5555");

	zmq::message_t request(6);
	// turn BIG to char for socket sending
	char cipher_big_to_char[800], cipher_recv[800];
	Big recv_cipher;
	cipher_big_to_char << cipher;
	cipher_big_to_char[800] = 0;
	memcpy((void*) request.data(), cipher_big_to_char, 800);
	// GET TO THE CHOPA! 
	std::cout << "Connected to server, sending cipher" << std::endl;
	socket.send(request);
	std::cout << "Cipher sent" << std::endl;
	

	// wait for reply
	zmq::message_t reply;
	socket.recv(&reply);
	std::cout << "Received cipher text" << std::endl;
	std::cout << reply.data() << std::endl;
	memcpy(cipher_recv, (void *) reply.data(), 800);
	recv_cipher = cipher_recv;
	return recv_cipher;	
}

