#define INTROSAT_INTERNAL

#include "ISL_Bootloader.h"

char message_buffer[] = "Bootloader Mode\n\r";

void EnterBootloader(void)
{
#ifdef BOOT_ADDR // If bootloader address is set -> platform is supported 
#ifdef ARDUINO_AVR_ATmega328
	#ifdef HAVE_HWSERIAL0
	// If Atmega with Serial available - try to print message
	Serial.println("Entering bootloader...");
	Serial.flush();
	#endif /** HAVE_HWSERIAL0 */

	eeprom_write_byte((uint8_t*)52, 0); // Set flag to stay in bootloader after reset
    cli();
    SP = RAMEND;
    SREG = 0;
    MCUSR = 0;
    asm volatile ("eor  __zero_reg__, __zero_reg__");
    asm volatile ("eor  r1, r1");
    // jump to after the bootloader checks the reset source
    asm volatile ("ijmp" ::"z" (BOOT_ADDR / 2));
    while(1);

#else

#if defined(STM32F103xB) // If F103 (most probably base IntroSat) - print message to UART1
	

    for (char *next_char = message_buffer; *next_char != 0; next_char++)
    {
        while (READ_BIT(USART1->SR, USART_SR_TXE) != (USART_SR_TXE))
        {
        }
        USART1->DR = *next_char;
    }

    HAL_Delay(200); // Delay to send data through UART?

    CLEAR_BIT(USART1->CR1, (USART_CR1_UE));
#endif /** STM32F103xx */

    /* Disable all interrupts */
	__disable_irq();

	/* Set the clock to the default state */
#ifdef USE_HAL_DRIVER
	HAL_RCC_DeInit();
#endif /** USE_HAL_DRIVER */
	
	/* Disable Systick timer */
	SysTick->CTRL = 0;
  	SysTick->LOAD = 0;
  	SysTick->VAL = 0;

	/* Clear Interrupt Enable Register & Interrupt Pending Register */
	for (uint8_t i = 0; i < sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]); i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}

	/* Re-enable all interrupts */
	__enable_irq();

	void (*SysMemBootJump)(void);
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BOOT_ADDR + 4)));
  	__set_MSP(*(uint32_t *)BOOT_ADDR);
  	SysMemBootJump();
#endif
#endif /** BOOT_ADDR */ 
}
