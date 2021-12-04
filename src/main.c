#include "invaders.h"
#include "lcd.h"
#include "sound.h"
#include "sprite_data.h"
#include "sprites.h"
#include "nunchuk.h"
#include "string.h"
#include "stm32f0xx.h"

// TODO This should be replaced with global tick, maybe use SysTick and check it
// rather than busy looping
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
  // Little test guy
  Sprite invader;
  init_sprite(40, 40, invader1_a_width, invader1_a_height, (uint16_t*)invader1_a, (uint16_t*)invader1_b, &invader);
  // Initialize the invader army
  init_invaders();
  // Init i2c and nunchuk
  init_nunchuk();
  for (;;) {
    print_nunchuk_xy(100,100);
    // Draw the test sprite
    draw_sprite(&invader);
    // Change ("animate") the test sprite
    invader.sprite_data =
        (uint16_t *)(((uint32_t)invader.sprite_data) ^ invader.sprite_swap_key);
    // Draw the invading army
    draw_invaders();
    // Animate the army
    update_invaders();
    // Wait like a dum-dum because we dont have a global tick setup yet
    nano_wait(500000000);
     asm volatile("wfi" ::);
  }
}
