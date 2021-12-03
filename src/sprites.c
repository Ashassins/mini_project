#include "sprites.h"
#include "lcd.h"

void draw_sprite(Sprite *s) {
  lcddev.select(1);
  LCD_SetWindow(s->x, s->y, s->x + s->width - 1, s->y + s->height - 1);
  LCD_WriteData16_Prepare();

  for (int y = s->height - 1; y >= 0; y--) {
    for (int x = s->width - 1; x >= 0; x--) {
      LCD_WriteData16(s->sprite_data[x + y * s->width]);
    }
  }

  LCD_WriteData16_End();
  lcddev.select(0);
}

// Gonna need this for the tank and others lmao
void move_sprite(Sprite *s, uint16_t mov_x, uint16_t mov_y) {
  // TODO
}
