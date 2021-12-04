#include "sprites.h"
#include "lcd.h"
#include "util.h"

void draw_sprite(Sprite *s) {
  lcddev.select(1);
  LCD_SetWindow(s->bbox.x1, s->bbox.y1, s->bbox.x2, s->bbox.y2);
  LCD_WriteData16_Prepare();

  uint16_t width = s->bbox.x2 - s->bbox.x1;
  uint16_t height = s->bbox.y2 - s->bbox.y1;
  uint16_t px_access = 0;
  for (int y = height; y >= 0; y--) {
    for (int x = width; x >= 0; x--) {
      px_access = x + y * (width + 1);
      LCD_WriteData16(s->sprite_data[px_access]);
    }
  }

  LCD_WriteData16_End();
  lcddev.select(0);
}


void init_sprite(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *sprite_a, uint16_t *sprite_b, Sprite *s) {
  s->bbox.x1 = x;
  s->bbox.y1 = y;
  s->bbox.x2 = x + width - 1;
  s->bbox.y2 = y + height - 1;
  if(sprite_a == sprite_b || !sprite_b) {
    s->sprite_swap_key = 0;
  } else {
    s->sprite_swap_key = ((uint32_t)sprite_a) ^ ((uint32_t)sprite_b);
  }
  s->sprite_data = sprite_a;
}

// Gonna need this for the tank and others lmao
void move_sprite(Sprite *s, int16_t mov_x, int16_t mov_y) {
/*
  // TODO
  Rect old, new, hull;
  old = s->bbox;
  new = s->bbox;
  new.x1 += mov_x;
  new.x2 += mov_x;
  new.y1 += mov_y;
  new.y2 += mov_y;
  compute_hull(old, new, &hull);

  uint16_t height = s->bbox.y2 - s->bbox.y1;
  uint16_t widht = s->bbox.x2 - s->bbox.x1;
  uint16_t x2 = s->bbox.x2, y2 = s->bbox.y2;

  for(uint16_t y = hull.y2; y >= hull.y1; y--) {
    for(uint16_t x = hull.x2; x >= hull.x1; x--) {

      int16_t draw_y = y2 - y;
      int16_t draw_x = x2 - x;

      if(draw_x >= 0 && draw_y >= 0) {
        //LCD_WriteData16
      } else {
        LCD_WriteData16(0x0);
      }

    }
  }
*/

lcddev.select(1);
LCD_SetWindow(s->bbox.x1, s->bbox.y1, s->bbox.x2, s->bbox.y2);
LCD_WriteData16_Prepare();

uint16_t width = s->bbox.x2 - s->bbox.x1;
for (int y = s->bbox.y2 - s->bbox.y1; y >= 0; y--) {
  for (int x = width; x >= 0; x--) {
	LCD_WriteData16(0);
  }
}

LCD_WriteData16_End();
lcddev.select(0);

s->bbox.x1 += mov_x;
s->bbox.x2 += mov_x;
s->bbox.y1 += mov_y;
s->bbox.y1 += mov_y;
draw_sprite(s);

}
