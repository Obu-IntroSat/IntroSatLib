#include "IS_Bluetooth.h"

char enter_buffer[] = "Bootloader Mode\n\r";

void enter_bootloader(void)
{

  for (char* next_char = enter_buffer; *next_char != 0; next_char++)
  {
    while (READ_BIT(USART1->SR, USART_SR_TXE) != (USART_SR_TXE)) {}
    USART1->DR = *next_char;
  }


  HAL_Delay(200);


  CLEAR_BIT(USART1->CR1, (USART_CR1_UE));

  void (*SysMemBootJump)(void);
  volatile uint32_t addr = 0x1FFFF000;

#ifdef USE_HAL_DRIVER
  HAL_RCC_DeInit();
#endif /* USE_HAL_DRIVER */

  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;

  __disable_irq();
  SysMemBootJump = (void (*)(void))(*((uint32_t *)(addr + 4)));
  __set_MSP(*(uint32_t *)addr);
  SysMemBootJump();
}
