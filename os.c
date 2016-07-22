#include <stdint.h>
#include "os.h"

uint32_t __osGetSR()
{
	asm volatile ("mfc0  $v0, $12 \
	jr    $ra \
	nop   ");
}

void osInitialize()
{
	
}
