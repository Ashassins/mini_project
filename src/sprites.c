#include "lcd.h"
#include "sprites.h"


void draw_sprite(Sprite *s) {
  lcddev.select(1);
  LCD_SetWindow(s->x, s->y, s->x+s->width-1, s->y+s->height-1);
  LCD_WriteData16_Prepare();
  for(int y = s->height - 1; y >= 0; y--) {
    for(int x = s->width - 1; x >= 0; x--) {
      LCD_WriteData16(s->sprite_data[y + x * s->height]);
      //LCD_WriteData16(0xffff);
    }
  }

  LCD_WriteData16_End();
  lcddev.select(0);
}

void move_sprite(Sprite *s, uint16_t mov_x, uint16_t mov_y) {

}
