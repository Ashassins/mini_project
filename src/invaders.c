#include "invaders.h"
#include "lcd.h"
#include "sprites.h"
#include "util.h"

InvaderArmy invader_army;
int _n_units = sizeof(invader_army.units) / sizeof(invader_army.units[0]);

// Initialize the invader state
void init_invaders() {
  uint16_t *sprite_a;
  uint16_t *sprite_b;
  for (int y = 0; y < INVADERS_HEIGHT; y++) {
    for (int x = 0; x < INVADERS_WIDTH; x++) {
      if (y <= 1) {
        sprite_a = (uint16_t *)invader1_a;
        sprite_b = (uint16_t *)invader1_b;
      } else if (y <= 3) {
        sprite_a = (uint16_t *)invader2_a;
        sprite_b = (uint16_t *)invader2_b;
      } else {
        sprite_a = (uint16_t *)invader3_a;
        sprite_b = (uint16_t *)invader3_b;
      }
      init_sprite(
          x * invader1_a_width,
          y * (invader1_a_height + 2), // Add two to acount for the spacing
          invader1_a_width, invader1_a_height, sprite_a, sprite_b,
          &invader_army.units[x + y * INVADERS_WIDTH]);
    }
  }
  invader_army.bbox.x1 = 20;
  invader_army.bbox.y1 = 230;
  invader_army.drop = 5;
  invader_army.step = 2;
  // The number of pixels we need to draw the invaders, accounting for the 2 px
  // between rows
  invader_army.bbox.x2 =
      invader_army.bbox.x1 + INVADERS_WIDTH * invader1_a_width;
  invader_army.bbox.y2 = invader_army.bbox.y1 +
                         INVADERS_HEIGHT * invader1_a_height +
                         (INVADERS_HEIGHT * 2) - 1;
}

/*
 * This function will most likely not get used,
 * however the core logic will most likely be useful for drawing the invaders
 * each frame as they move NOTE things get completly screwed if any of the
 * invaders have a different size
 */
void draw_invaders() {
  // Start the draw call
  lcddev.select(1);
  LCD_SetWindow(invader_army.bbox.x1, invader_army.bbox.y1,
                invader_army.bbox.x2, invader_army.bbox.y2);
  LCD_WriteData16_Prepare();

  // Draw the army
  uint16_t sprite_x = 0, sprite_y = 0;
  uint16_t army_idx = 0;
  // Loop bounds
  uint16_t start_y = invader_army.bbox.y1;
  uint16_t end_y = invader_army.bbox.y2;
  uint16_t start_x = invader_army.bbox.x1;
  uint16_t end_x = invader_army.bbox.x2;

  for (uint16_t glob_y = start_y; glob_y <= end_y; glob_y++) {
    // Do we need to draw between the rows
    if (sprite_y >= invader1_a_height) {
      // Draw some spacing between the rows
      glob_y += 2;
      for (int i = 0; i < (end_x - start_x + 1) * 2; i++) {
        LCD_WriteData16(0x0);
      }
      // Reset the sprite veritcal
      sprite_y = 0;
      army_idx += INVADERS_WIDTH;
    }
    // Go through a row
    for (uint16_t glob_x = start_x; glob_x <= end_x; glob_x++) {
      uint16_t *cur_data = invader_army.units[0].sprite_data;
      if (cur_data) {
        LCD_WriteData16(cur_data[sprite_x + sprite_y * invader1_a_width]);
      } else {
        LCD_WriteData16(0x0);
      }
      sprite_x++;
      if (sprite_x >= invader1_a_width) {
        sprite_x = 0;
        army_idx += 1;
      }
    }
    army_idx -= INVADERS_WIDTH;
    sprite_y++;
    sprite_x = 0;
  }
  // Close down the communication with the lcd
  LCD_WriteData16_End();
  lcddev.select(0);
}

// TODO Move the invaders by step, when they hit the edge of the screen make
// step negative
// TODO Clear the region where the invaders were but no longer are
void update_invaders() {
  for (int i = 0; i < _n_units; i++) {
    // Swap each sprite pointer
    invader_army.units[i].sprite_data =
        (uint16_t *)(((uint32_t)invader_army.units[i].sprite_data) ^
                     invader_army.units[i].sprite_swap_key);
  }
  Rect hull, old, new;
  old = invader_army.bbox;
  new = invader_army.bbox;
  uint16_t new_left = old.x2 + invader_army.step;
  if(new_left >= LCD_W || old.x1 + invader_army.step < 0) {
    invader_army.step = -invader_army.step;
    new.y1 = old.y1 - invader_army.drop;
    new.y2 = old.y2 - invader_army.drop;
  } else {
    new.x1 = old.x1 + invader_army.step;
    new.x2 = old.x2 + invader_army.step;
    new.y1 = old.y1;
    new.y2 = old.y2;
  }  compute_hull(old, new, &hull);
  invader_army.bbox = new;

  // Start the draw call
  lcddev.select(1);
  LCD_SetWindow(hull.x1, hull.y1, hull.x2, hull.y2);
  LCD_WriteData16_Prepare();

  // Draw the army
  uint16_t sprite_x = 0, sprite_y = 0;
  uint16_t army_idx = 0;

  //// Draw the army
  for (uint16_t glob_y = hull.y1; glob_y <= hull.y2; glob_y++) {
    // Do we need to draw between the rows
    if (sprite_y >= invader1_a_height) {
      // Draw some spacing between the rows
      glob_y += 2;
      for (int i = 0; i < (hull.x2 - hull.x1 + 1) * 2; i++) {
        LCD_WriteData16(0x0);
      }
      // Reset the sprite veritcal
      sprite_y = 0;
      army_idx += INVADERS_WIDTH;
    }
    // Go through a row
    for (uint16_t glob_x = hull.x1; glob_x <= hull.x2; glob_x++) {
      if (contains(glob_x, glob_y, new)) {
        uint16_t *cur_data = invader_army.units[0].sprite_data;
        if (cur_data) {
          LCD_WriteData16(cur_data[sprite_x + sprite_y * invader1_a_width]);
        } else {
          LCD_WriteData16(0x0);
        }
        sprite_x++;
        if (sprite_x >= invader1_a_width) {
          sprite_x = 0;
          army_idx += 1;
        }
      } else {
        LCD_WriteData16(0x0);
      }
    }
    army_idx -= INVADERS_WIDTH;
    sprite_y++;
    sprite_x = 0;
  }
  //// Close down the communication with the lcd
  LCD_WriteData16_End();
  lcddev.select(0);
}
