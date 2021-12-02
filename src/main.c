#include "lcd.h"
#include "sound.h"
#include "stm32f0xx.h"
#include "sprite_data.h"
#include "sprites.h"
#include "invaders.h"

static inline void nano_wait(unsigned int n) {
  asm("        mov r0,%0\n"
      "repeat: sub r0,#83\n"
      "        bgt repeat\n"
      :
      : "r"(n)
      : "r0", "cc");
}

int main(void) {
  setup_music();
  start_music();
  LCD_Setup();
  LCD_Clear(0x0);
  Sprite invader = {
    .x = 40,
    .y = 40,
    .height = invader1_a_height,
    .width = invader1_a_width,
    .sprite_data = (uint16_t *) invader1_a,
    .sprite_swap_key = ((uint32_t) invader1_a) ^ ((uint32_t)invader1_b)
  };
  for (;;) {
    draw_sprite(&invader);
    invader.sprite_data = (uint16_t *) (((uint32_t) invader.sprite_data) ^ invader.sprite_swap_key);
    nano_wait(500000000);
    //asm volatile("wfi" ::);
  }
}
