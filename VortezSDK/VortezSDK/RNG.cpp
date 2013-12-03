#include "RNG.h"

//
CRNG::CRNG()
{
	InitializeGenerator(GetTickCount());
}

// Initialize the generator from a seed
void CRNG::InitializeGenerator(int Seed)
{
	index = 0;
	MT[0] = Seed;
	for(int i = 1; i < 624; i++){ // loop over each other element
		MT[i] = (0x6C078965 * (MT[i-1] ^ (MT[i-1] >> 30)) + i);
	}
}


// Extract a tempered pseudorandom number based on the index-th value,
// calling generateNumbers() every 624 numbers
int CRNG::ExtractNumber()
{
	if(index == 0)
		GenerateNumbers();
	
	int y = MT[index];
	y = y ^ (y >> 11);
	y = y ^ ((y << 7)  & (0x9D2C5680));
	y = y ^ ((y << 15) & (0xEFC60000));
	y = y ^ (y >> 18);
	
	index = (index + 1) % 624;
	return y;
}

// Generate an array of 624 untempered numbers
void CRNG::GenerateNumbers()
{
	for(int i = 0; i < 624; i++){ 
		
		int y = ((MT[i] & 0x80000000) + (MT[(i+1) % 624] & 0x7FFFFFFF));
		
		MT[i] = MT[(i + 397) % 624] ^ (y >> 1);
		
		if((y % 2) != 0) // y is odd
			MT[i] = MT[i] ^ (0x9908B0DF);
	}
}