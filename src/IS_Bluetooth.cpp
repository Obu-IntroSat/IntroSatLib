#include "IS_Bluetooth.h"

void enter_bootloader(void)
{

  HAL_Delay(200);

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
