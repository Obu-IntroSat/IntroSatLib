#ifndef HELPERS_HARDFAULT_H_
#define HELPERS_HARDFAULT_H_

#include "../Includes/Base.h"

namespace IntroSatLib {
namespace Helpers {

struct Registers
{
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r12;
	uint32_t lr;
	uint32_t pc;
	uint32_t psr;
};

#ifdef USE_STACK
#define get_stack(result) asm( "TST lr, #4 \n"\
		"ITE EQ \n"\
		"MRSEQ %[ptr], MSP  \n"\
		"MRSNE %[ptr], PSP  \n"\
		: [ptr] "=r" (result)\
)
#else
#define get_stack(result) result = NULL
#endif

void print_hard_fault(Registers* result = NULL)
{
	if (result != NULL)
	{
		printf("R0  \t0x%4lX", result->r0);
		printf("R1  \t0x%4lX", result->r1);
		printf("R2  \t0x%4lX", result->r2);
		printf("R3  \t0x%4lX", result->r3);
		printf("R12 \t0x%4lX", result->r12);
		printf("LR  \t0x%4lX", result->lr);
		printf("PC  \t0x%4lX", result->pc);
		printf("PSR \t0x%4lX", result->psr);
	}
	printf("R0  \t0x%4lX", SCB->HFSR);
	printf("BFAR\t0x%4lX", SCB->BFAR);
	printf("UFSR\t0x%4lX", SCB->SCR);
}

} /* namespace Helpers */
} /* namespace IntroSatLib */


#endif /* HELPERS_HARDFAULT_H_ */
