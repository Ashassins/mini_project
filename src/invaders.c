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
  // Rect hull, old, new;
  // old = invader_army.bbox;
  // new = invader_army.bbox;
  // uint16_t new_left = old.x2 + invader_army.step;
  // if(new_left > LCD_W || new_left < 0) {
  //  invader_army.step = -invader_army.step;
  //  new.y1 = old.y1 - invader_army.drop;
  //  new.y2 = old.y2 - invader_army.drop;
  //} else {
  //  new.x1 = old.x1 + invader_army.step;
  //  new.x2 = old.x2 + invader_army.step;
  //  new.y1 = old.y1;
  //  new.y2 = old.y2;
  //}
  // compute_hull(old, new, &hull);

  // invader_army.x = new.x1;
  // invader_army.y = new.y1;
  //// Start the draw call
  // lcddev.select(1);
  // LCD_SetWindow(invader_army.x, invader_army.y, invader_army.x +
  // invader_army.width - 1,
  //              invader_army.y + invader_army.height - 1);
  // LCD_WriteData16_Prepare();

  //// Draw the army
  // for (int army_y = INVADERS_HEIGHT - 1; army_y >= 0; army_y--) {
  //  for (int sprite_y = invader1_a_height - 1; sprite_y >= 0; sprite_y--) {
  //    for (int army_x = INVADERS_WIDTH - 1; army_x >= 0; army_x--) {
  //      uint8_t invader_type = invader_army.units[army_x + army_y *
  //      INVADERS_WIDTH]; for (int sprite_x = invader1_a_width - 1; sprite_x >=
  //      0; sprite_x--) {
  //        // Select which sprite to draw from
  //        switch (invader_type) {
  //        case 1:
  //          LCD_WriteData16(
  //              invader_army
  //                  .unit_sprites[0][sprite_x + sprite_y * invader1_a_width]);
  //          break;
  //        case 2:
  //          LCD_WriteData16(
  //              invader_army
  //                  .unit_sprites[1][sprite_x + sprite_y * invader1_a_width]);
  //          break;
  //        case 3:
  //          LCD_WriteData16(
  //              invader_army
  //                  .unit_sprites[2][sprite_x + sprite_y * invader1_a_width]);
  //          break;
  //        default:
  //          LCD_WriteData16(0);
  //        }
  //      }
  //    }
  //  }
  //  // Row spacing, just a bunch of black pixels
  //  for (int x = (invader1_a_width * INVADERS_WIDTH * 2) - 1; x >= 0; x--) {
  //    LCD_WriteData16(0);
  //  }
  //}
  //// Close down the communication with the lcd
  // LCD_WriteData16_End();
  // lcddev.select(0);
}
