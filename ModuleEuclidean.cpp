#include "Arduino.h"
#include "ModuleEuclidean.h"
#include "defines.h"
 
ModuleEuclidean::ModuleEuclidean()
{
	this->step = 0;
	this->old_clock = 0;
	this->old_on = 0;
	this->old_total = 0;

	
	// Initialize all inputs
	this->clock_input = NULL; 
	this->on_input = NULL;
	this->total_input = NULL;
}
 
uint16_t ModuleEuclidean::compute()
{
	uint16_t clock = this->readInput(clock_input);
	uint16_t on = this->readInput(on_input);
	uint16_t total = this->readInput(total_input);
	
	if ((old_on != on) || (old_total != total))
	{
		sequence = euclid(on, total);
	}
	
	// Step the sequencer on the rising edge 
	if((clock > MID_CV) && (old_clock < MID_CV))
	{
		step++;
		if(step >= total) step = 0;
	}
	  
	old_clock = clock;	
	old_on = on;
	old_total = total;

	return(sequence[step]);
}

// Euclid calculation function by Tom Whitwell
unsigned int euclid(int n, int k){ // inputs: n=total, k=beats, o = offset
	int pauses = n-k;
	int pulses = k;
	int per_pulse = pauses/k;
	int remainder = pauses%pulses;  
	unsigned int workbeat[n];
	unsigned int outbeat;
	unsigned int working;
	int workbeat_count=n;
	int a; 
	int b; 
	int trim_count;
	for (a=0;a<n;a++){ // Populate workbeat with unsorted pulses and pauses 
		if (a<pulses){
			workbeat[a] = 1;
		}
		else {
			workbeat [a] = 0;
		}
	}

	if (per_pulse>0 && remainder <2){ // Handle easy cases where there is no or only one remainer  
		for (a=0;a<pulses;a++){
			for (b=workbeat_count-1; b>workbeat_count-per_pulse-1;b--){
				workbeat[a]  = ConcatBin (workbeat[a], workbeat[b]);
			}
			workbeat_count = workbeat_count-per_pulse;
		}

		outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count 
		for (a=0;a < workbeat_count;a++){
			outbeat = ConcatBin(outbeat,workbeat[a]);
		}
		return outbeat;
	}

	else { 

		int groupa = pulses;
		int groupb = pauses; 
		int iteration=0;
		if (groupb<=1){
		}
		while(groupb>1){ //main recursive loop

			if (groupa>groupb){ // more Group A than Group B
				int a_remainder = groupa-groupb; // what will be left of groupa once groupB is interleaved 
				trim_count = 0;
				for (a=0; a<groupa-a_remainder;a++){ //count through the matching sets of A, ignoring remaindered
					workbeat[a]  = ConcatBin (workbeat[a], workbeat[workbeat_count-1-a]);
					trim_count++;
				}
				workbeat_count = workbeat_count-trim_count;

				groupa=groupb;
				groupb=a_remainder;
			}


			else if (groupb>groupa){ // More Group B than Group A
				int b_remainder = groupb-groupa; // what will be left of group once group A is interleaved 
				trim_count=0;
				for (a = workbeat_count-1;a>=groupa+b_remainder;a--){ //count from right back through the Bs
					workbeat[workbeat_count-a-1] = ConcatBin (workbeat[workbeat_count-a-1], workbeat[a]);

					trim_count++;
				}
				workbeat_count = workbeat_count-trim_count;
				groupb=b_remainder;
			}


			else if (groupa == groupb){ // groupa = groupb 
				trim_count=0;
				for (a=0;a<groupa;a++){
					workbeat[a] = ConcatBin (workbeat[a],workbeat[workbeat_count-1-a]);
					trim_count++;
				}
				workbeat_count = workbeat_count-trim_count;
				groupb=0;
			}

			else {
			//        Serial.println("ERROR");
			}
			iteration++;
		}


		outbeat = 0; // Concatenate workbeat into outbeat - according to workbeat_count 
		for (a=0;a < workbeat_count;a++){
		outbeat = ConcatBin(outbeat,workbeat[a]);
		}

		return outbeat;
	}
}

// Function to find the binary length of a number by counting bitwise 
int findlength(unsigned int bnry){
	boolean lengthfound = false;
	int length=1; // no number can have a length of zero - single 0 has a length of one, but no 1s for the sytem to count
	for (int q=32;q>=0;q--){
		int r=bitRead(bnry,q);
		if(r==1 && lengthfound == false){
			length=q+1;
			lengthfound = true;
		}
	}
	return length;
}

// Function to concatenate two binary numbers bitwise 
unsigned int ConcatBin(unsigned int bina, unsigned int binb){
	int binb_len=findlength(binb);
	unsigned int sum=(bina<<binb_len);
	sum = sum | binb;
	return sum;
}