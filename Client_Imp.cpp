#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include <ctime>

#include"PEKS_Test.h"

Storage Read_File(){
	
	Storage store;
	// init variables

	store.text_file = new string[BUFFER];
	store.keyword   = new string[BUFFER];

	ifstream my_file ("160_words.txt");

	if(my_file.is_open()){
		// read file 
		for(int i =0; i < BUFFER; i++){
			my_file >> store.text_file[i];
			// place each word in file into keyword array
			store.keyword[i] = store.text_file[i];
		}	
	} else {

		printf("Your file cannot be opened\n");
		exit(0);
	}

	my_file.close();
	return store;
}

Storage Client::Encryption(PFC &pfc, Big r, G1 h, G1 g){
	// randomization initialization 
	time_t seed;
	time(&seed);	
	irand((long)seed);
	// read in file 
	Storage store  = Read_File();

	GT t;	
	clock_t t_encrypt;
	t_encrypt = clock();
	for(int i = 0; i < BUFFER; ++i){
		pfc.random(r);
		// store each keyword into a c style array
		string temp = store.keyword[i];
		char temp_a[1024];
		strncpy(temp_a, temp.c_str(), sizeof(temp_a));
		// perform encryption 
		pfc.hash_and_map(store.HW[i], temp_a);	
		// generate pairing token t
		t = pfc.pairing(store.HW[i], pfc.mult(h,r));	
		// store ciphertext pair
		store.PA[i] = pfc.mult(g,r);		
		store.PB[i] = pfc.hash_to_aes_key(t);
	}
	t_encrypt = clock() - t_encrypt;
	printf("Time to encrypt %d words: %f\n", BUFFER, (((float)t_encrypt) / CLOCKS_PER_SEC));

	return store;	
}	
	

