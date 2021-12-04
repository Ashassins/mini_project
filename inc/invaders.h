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
  // The bounding box for the invaders
  Rect bbox;
  // How the army will move
  int8_t step, drop;
  // Each sprite for the invaders
  Sprite units[INVADERS_COUNT];
} InvaderArmy;

extern InvaderArmy invader_army;

void init_invaders();
void draw_invaders();
void update_invaders();

#endif
