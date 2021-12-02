#ifndef INVADERS_H
#define INVADERS_H
#include "lcd.h"
#include "sprite_data.h"

#define INVADERS_WIDTH 11
#define INVADERS_HEIGHT 5
#define INVADERS_COUNT (INVADERS_HEIGHT * INVADERS_WIDTH)
// Invader a, b, c, + explosion
#define INVADERS_SPRITE_COUNT 4

typedef struct {
  uint16_t x,y;
  uint16_t width, height;
  int8_t step;
  uint8_t units[INVADERS_COUNT];
  uint16_t *unit_sprites[INVADERS_SPRITE_COUNT];
  uint32_t unit_sprite_switch_keys[INVADERS_SPRITE_COUNT];
} Invaders;

extern Invaders invaders;

void init_invaders();
void draw_invaders();
void update_invaders();

#endif
