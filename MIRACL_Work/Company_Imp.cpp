#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>

#include "PEKS_Test.h"

G1 Company::Gen_Generator(PFC &pfc, G1 g){

	time_t 	seed;
	time(&seed);
	irand((long)seed);
	
	pfc.random(g);
	
	return g;
}

Trap Company :: Gen_Trap(PFC &pfc, Big alpha, Storage store){

	Trap trap; 	

	for(int i = 0; i < BUFFER; ++i){
		// turn string into key array
		string temp = store.keyword[i];
		char temp_a[1024];
		strncpy(temp_a, temp.c_str(), sizeof(temp_a));
		// encrypt
		pfc.hash_and_map(store.HW[i], temp_a);
		trap.TW[i] = pfc.mult(store.HW[i], alpha);
		pfc.precomp_for_pairing(trap.TW[i]);
	}

	return trap;
}

Big Company::Gen_Priv_Key(PFC &pfc, Big alpha){

	time_t 	seed;
	time(&seed);
	irand((long)seed);
	// randomly generate private key 
	pfc.random(alpha);
	
	return alpha;
}

G1 Company::Gen_Pub_key(PFC &pfc, Big alpha, G1 g){

	G1 h;
	h = pfc.mult(g, alpha);
	
	return h;
}

void Company::Querry_Data(PFC &pfc, Storage store, Trap trap){
	clock_t t_start;
	double time_elapsed;
	t_start = clock();	
	for(int i = 0; i < BUFFER; ++i){

		printf("starting %d\n", i);
	    // restart while loop counter	
		int j;
		j = 0;
	   // pairing for all trap door, must divide total time with number or KEYWORDS!!!!	
		while(j < BUFFER){	
	
			clock_t t_pairing;
			t_pairing = clock();
			G2 trap_temp = trap.TW[j];
			// start timing
			// perform pairing verification	
			if(pfc.hash_to_aes_key(pfc.pairing(trap_temp, 
								   store.PA[j])) == store.PB[i]){

				clock_t stop = clock();
				printf("Trap door at array index %d matches Pairing at %d\n", j, i);
				// calculating time of pairing operation 
				time_elapsed = stop - t_pairing;					
				printf("Time for pairing 1 keyword: %f\n", (((time_elapsed) /(double)CLOCKS_PER_SEC)));
				break;
			} else {
				clock_t stop = clock();
				time_elapsed = ((double)(stop- t_pairing)) / (double)CLOCKS_PER_SEC;	
							
				printf("Trap door array at index %d does not match Pairing at %d\n", j, i);
				printf("time for pairing: %f\n", time_elapsed);
			}
			j = j+ 1;
		}

		printf("Outside of Pairing Loop\n");
	}

	t_start = clock() - t_start;
	printf("It took %f seconds to perform entire pairing\n", (((float)t_start)/ CLOCKS_PER_SEC));
}
