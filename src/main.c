#include "lcd.h"
#include "sound.h"
#include "stm32f0xx.h"

int main(void) {
  setup_music();
  //start_music();
  LCD_Setup();
  LCD_Clear(0x7e0);
  for (;;) {
    asm volatile("wfi" ::);
  }
}
