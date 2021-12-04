#include "invaders.h"
#include "lcd.h"
#include "sound.h"
#include "sprite_data.h"
#include "sprites.h"
#include "nunchuk.h"
#include "timer.h"
#include "string.h"
#include "stm32f0xx.h"

#define SHOT_SPEED 5

int score = 0;
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

void print_glbcnt(int x, int y) {
	int toggle = 0;
    char output[5] = "G:  ";
    itoa(glbcnt, &output[2], 10);
    LCD_DrawString(x,y,0xFFFF,0x0000, output, 16, 0);
}

void draw_score(int x, int y) {
    char output[10] = "SCORE:   ";
    itoa(score, &output[6], 10);
    LCD_DrawString(x,y,0xFFFF,0x0000, output, 16, 0);
}

int main(void) {
  init_nunchuk();
  init_tim6();
  setup_music();
  start_music();
  LCD_Setup();
  LCD_Clear(0x0);
  // Little test guy
//  Sprite invader;
  Sprite player;
  Sprite shot;
  Sprite bunker;
//  init_sprite(40, 40, invader1_a_width, invader1_a_height, (uint16_t*)invader1_a, (uint16_t*)invader1_b, &invader);
  init_sprite(120,25,tank_clean_width, tank_clean_height, (uint16_t*)tank_clean, (uint16_t*)tank_clean, &player);
  init_sprite(1000,1000,tank_shot_width, tank_shot_height, (uint16_t*)tank_shot, (uint16_t*)tank_shot, &shot);
  init_sprite(100,100,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &bunker);
  // Initialize the invader army
  init_invaders();
  // Init i2c and nunchuk
  //
//  uint16_t mov_x = 5, mov_y = 5;
  for (;;) {
    if ((glbcnt + 1) % 15 == 0) {
    	// -----4FPS (BASICALLY FREE)-----
//    	LCD_DrawString(230,300,0x0F00,0x0000, "Don't mind the spaz monke uwu", 16, 0);
    	draw_score(140,300);
        // Change ("animate") the test sprite
    	player.sprite_data =
            (uint16_t *)(((uint32_t)player.sprite_data) ^ player.sprite_swap_key);
//    	invader.sprite_data =
//            (uint16_t *)(((uint32_t)invader.sprite_data) ^ invader.sprite_swap_key);
    	draw_sprite(&bunker);
    	// Draw the invading army
        draw_invaders();
        // Animate the army
        update_invaders();
    }

    if ((glbcnt + 1) % 4 == 0) {
    	// -----15FPS-----
        print_flags(175,1);
//        print_glbcnt(230,1);
    }

    if ((glbcnt + 1) % 2 == 0) {
		// -----30FPS-----
        if (flg_mv_right && player.bbox.x1 > 0) {
        	move_sprite(&player, -2, 0, 0);
        } else if (flg_mv_left && player.bbox.x2 < (LCD_W - 1)) {
        	move_sprite(&player, 2, 0, 0);
        } else {
        	draw_sprite(&player);
        }
        if (flg_v && (shot.bbox.x1 == 1000)) {
            teleport_sprite((int)((player.bbox.x2 + player.bbox.x1) / 2), player.bbox.y1 + 10, &shot);
            draw_sprite(&shot);
        }
        if (shot.bbox.y2 >= (LCD_H + SHOT_SPEED)) {
            teleport_sprite(1000, 1000, &shot);
        } else if (shot.bbox.x1 != 1000) {
            move_sprite(&shot, 0, SHOT_SPEED, 0);
        }

        // Collision test
        if (sprite_coll(&shot, &bunker)) {
            LCD_DrawString(200,200,0xF000,0x0000, "collision", 16, 0);
            teleport_sprite(1000, 1000, &shot);
            score += 1;
        } else {
            LCD_DrawString(200,200,0xF000,0x0000, "         ", 16, 0);
        }
	}

	// -----60FPS (VERY COSTLY)-----

//    move_sprite(&invader, mov_x, mov_y, 0);
//    if(invader.bbox.x1 <= 0 || invader.bbox.x2 >= (LCD_W - 5)) {
//	  mov_x = -mov_x;
//    }
//    if(invader.bbox.y1 <= 0 || invader.bbox.y2 >= LCD_H){
//	  mov_y = - mov_y;
//    }

    while((glbcnt + 1) % 1 != 0);
    asm volatile("wfi" ::);
  }
}
