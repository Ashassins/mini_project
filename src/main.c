#include "invaders.h"
#include "lcd.h"
#include "sound.h"
#include "sprite_data.h"
#include "sprites.h"
#include "nunchuk.h"
#include "timer.h"
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

//// Global counter, modified in timer interrupt
//int glbcnt = 0;
//
//// Timer initializer and interrupt
//void init_tim6() {
//    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
//    TIM6->PSC = 1600 - 1;
//    TIM6->ARR = 1000 - 1; // (48000000 / 1600) / 1000 = 30
//    TIM6->DIER |= TIM_DIER_UIE;
//    TIM6->CR1 |= TIM_CR1_CEN;
//    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;
//}
//
//void TIM6_DAC_IRQHandler(void) {
//    TIM6->SR &= ~TIM_SR_UIF;
//    glbcnt++;
//    if(glbcnt > 30) glbcnt = 0;
//    update_flags();
//}

int main(void) {
  init_nunchuk();
  init_tim6();
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
  //
  for (;;) {
     //print_nunchuk_xy(100,100);
    // Draw the test sprite
    print_flags(100,100);
    draw_sprite(&invader);
    // Change ("animate") the test sprite
    invader.sprite_data =
        (uint16_t *)(((uint32_t)invader.sprite_data) ^ invader.sprite_swap_key);
    // Draw the invading army
    draw_invaders();
    // Animate the army
    update_invaders();
    // Wait until global counter hits correct value
    while((glbcnt + 1) % 15 != 0);
    asm volatile("wfi" ::);
  }
}
