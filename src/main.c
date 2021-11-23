#include "sound.h"
#include "stm32f0xx.h"

int main(void) {
  setup_music();
  start_music();
  for (;;) {
    asm volatile("wfi" ::);
  }
}
