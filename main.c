#include <stdint.h>
#include "includes.h"

void MainFunc()
{
	osInitialize();
	// Dummy routine was here
}

void main() // EntryPoint
{
	// Initialize some memory region?
	uint32_t base = 0x8033A580;
	for(uint32_t i=0; i<0x2CEE0; i+=8)
	{
		*(uint32_t*)(base+i) = 0;
		*(uint32_t*)(base+i+4) = 0;
	}
	// Jump to our actual "Main" function
	MainFunc();
}
