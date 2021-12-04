#ifndef INVADERS_H
#define INVADERS_H
#include "lcd.h"
#include "sprite_data.h"
#include "sprites.h"

#define INVADERS_WIDTH 11
#define INVADERS_HEIGHT 5
#define INVADERS_COUNT (INVADERS_HEIGHT * INVADERS_WIDTH)
// Invader a, b, c, + explosion
#define INVADERS_SPRITE_COUNT 4


typedef struct {
  // This is the bottom right corner of the
  uint16_t x, y;
  // Size of the army
  uint16_t width, height;
  // How the army will move
  int8_t step, drop;
  // The state of each unit in the army
  uint8_t units[INVADERS_COUNT];
  // Pointers to each unit's sprite data
  uint16_t *unit_sprites[INVADERS_SPRITE_COUNT];
  // Evil bit fuckery
  // Two sprite pointers xor'd to to toggle sprites
  uint32_t unit_sprite_switch_keys[INVADERS_SPRITE_COUNT];
} InvaderArmy;

extern InvaderArmy invader_army;

void init_invaders();
void draw_invaders();
void update_invaders();

#endif
