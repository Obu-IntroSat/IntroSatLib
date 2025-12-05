#include "IS_Bluetooth.h"

// This methos exists for backwards-compatibility
void enter_bootloader(void)
{
    // Use more universal method that supports multiple chips
    EnterBootloader(); 

}
