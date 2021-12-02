#ifndef SPRITES_H
#define SPRITES_H
#include "stm32f0xx_dac.h"
#include <stdint.h>

typedef struct {
  // The bottom right corner of the sprite
  uint16_t x,y;
  // The offset to the top left corner of the sprite from x,y
  uint16_t width, height;
  // The pointer to the data to draw the sprite
  // The data is laid out in Row-major order
  uint16_t *sprite_data;
  // This is computed as sprite_data_a ^ sprite_data_b
  // it get's xored with sprite_data to swich the sprite
  // If there is no alternate sprite, this will be zero
  uint32_t sprite_swap_key;
} Sprite;

void draw_sprite(Sprite *s);
void move_sprite(Sprite *s, uint16_t mov_x, uint16_t mov_y);

#endif
