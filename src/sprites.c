#include "sprites.h"
#include "lcd.h"
#include "util.h"

void draw_sprite(Sprite *s) {
  move_sprite(s, 0, 0, 1);
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
void move_sprite(Sprite *s, int16_t mov_x, int16_t mov_y, uint8_t force) {
  Rect old, new, hull;
  old = s->bbox;
  new = s->bbox;

  if(mov_x != 0 || mov_y != 0  || force) {
    new.x1 += mov_x;
    new.x2 += mov_x;
    new.y1 += mov_y;
    new.y2 += mov_y;
    compute_hull(old, new, &hull);
  }
  // Set the new bounding box
  s->bbox = new;

  if(hull.x1 < 0 || hull.y1 < 0 || hull.x2 > LCD_W || hull.y2 > LCD_H) {
    return;
  }

  // Magic screen incatation
  lcddev.select(1);
  LCD_SetWindow(hull.x1, hull.y1, hull.x2, hull.y2);
  LCD_WriteData16_Prepare();


  // Start drawing the sprite
  uint16_t paint_idx = 0;
  for(uint16_t y = hull.y1; y <= hull.y2; y++) {
    for(uint16_t x = hull.x1; x <= hull.x2; x++) {
      // Only get sprite data if we're drawing the sprite itself
      if(contains(x, y, new)) {
        LCD_WriteData16(s->sprite_data[paint_idx]);
        paint_idx++;
      } else { //Otherwise we just want to clear the section where it was
        LCD_WriteData16(0x0);
      }
    }
  }

  LCD_WriteData16_End();
  lcddev.select(0);

}

int sprite_coll(Sprite *s1, Sprite *s2) {
    if ((s1->bbox.x1 < 0 || s1->bbox.y1 < 0 || s1->bbox.x2 > LCD_W || s1->bbox.y2 > LCD_H) || (s2->bbox.x1 < 0 || s2->bbox.y1 < 0 || s2->bbox.x2 > LCD_W || s2->bbox.y2 > LCD_H)) {
        return 0;
    }
    return overlap(s1->bbox, s2->bbox);
}

void teleport_sprite(uint16_t x, uint16_t y, Sprite *s) {
    // x2 = x + width - 1;
    // x2 - x = width - 1;
    // x2 - x + 1 = width;
    int width = s->bbox.x2 - s->bbox.x1 + 1;
    int height = s->bbox.y2 - s->bbox.y1 + 1;
    s->bbox.x1 = x;
    s->bbox.y1 = y;
    s->bbox.x2 = x + width - 1;
    s->bbox.y2 = y + height - 1;
}
