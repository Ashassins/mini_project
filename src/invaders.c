#include "invaders.h"
#include "lcd.h"
#include "util.h"

InvaderArmy invader_army;
int _n_unit_type =
    sizeof(invader_army.unit_sprites) / sizeof(invader_army.unit_sprites[0]);


// Initialize the invader state
void init_invaders() {
  uint8_t type = 1;
  for (int i = 0; i < INVADERS_HEIGHT; i++) {
    for (int j = 0; j < INVADERS_WIDTH; j++) {
      if (i < 0) {
        type = 1;
      } else if (i <= 3) {
        type = 2;
      } else {
        type = 3;
      }
      invader_army.units[j + i * INVADERS_WIDTH] = type;
    }
  }
  invader_army.x = 0;
  invader_army.y = 230;

  // The number of pixels we need to draw the invaders, accounting for the 2 px
  // between rows
  invader_army.height = INVADERS_HEIGHT * invader1_a_height + (INVADERS_HEIGHT * 2);
  invader_army.width = INVADERS_WIDTH * invader1_a_width;

  // Init the array of sprite data pointers
  invader_army.unit_sprites[0] = (uint16_t *) invader1_a;
  invader_army.unit_sprites[1] = (uint16_t *) invader2_a;
  invader_army.unit_sprites[2] = (uint16_t *) invader3_a;
  invader_army.unit_sprites[3] = (uint16_t *) invader_explode;

  // Create our switch keys to "animate" the invaders
  invader_army.unit_sprite_switch_keys[0] =
      ((uint32_t)invader1_a) ^ ((uint32_t)invader1_b);
  invader_army.unit_sprite_switch_keys[1] =
      ((uint32_t)invader2_a) ^ ((uint32_t)invader2_b);
  invader_army.unit_sprite_switch_keys[2] =
      ((uint32_t)invader3_a) ^ ((uint32_t)invader3_b);
  // This one is zero because we only have one frame for the explosion
  invader_army.unit_sprite_switch_keys[3] = 0;
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
  LCD_SetWindow(invader_army.x, invader_army.y, invader_army.x + invader_army.width - 1,
                invader_army.y + invader_army.height - 1);
  LCD_WriteData16_Prepare();

  // Draw the army
  for (int army_y = 0; army_y < INVADERS_HEIGHT; army_y++) {
    for (int sprite_y = 0; sprite_y < invader1_a_height; sprite_y++) {
      for (int army_x = 0; army_x < INVADERS_WIDTH; army_x++) {
        uint8_t invader_type = invader_army.units[army_x + army_y * INVADERS_WIDTH];
        for (int sprite_x = 0; sprite_x < invader1_a_width; sprite_x++) {
          // Select which sprite to draw from
          switch (invader_type) {
          case 1:
            LCD_WriteData16(
                invader_army
                    .unit_sprites[0][sprite_x + sprite_y * invader1_a_width]);
            break;
          case 2:
            LCD_WriteData16(
                invader_army
                    .unit_sprites[1][sprite_x + sprite_y * invader1_a_width]);
            break;
          case 3:
            LCD_WriteData16(
                invader_army
                    .unit_sprites[2][sprite_x + sprite_y * invader1_a_width]);
            break;
          default:
            LCD_WriteData16(0);
          }
        }
      }
    }
    // Row spacing, just a bunch of black pixels
    for (int x = (invader1_a_width * INVADERS_WIDTH * 2) - 1; x >= 0; x--) {
      LCD_WriteData16(0);
    }
  }
  // Close down the communication with the lcd
  LCD_WriteData16_End();
  lcddev.select(0);
}

// TODO Move the invaders by step, when they hit the edge of the screen make
// step negative
// TODO Clear the region where the invaders were but no longer are
void update_invaders() {
  for (int i = 0; i < _n_unit_type; i++) {
    // Swap each sprite pointer
    invader_army.unit_sprites[i] =
        (uint16_t *)(((uint32_t)invader_army.unit_sprites[i]) ^
                     invader_army.unit_sprite_switch_keys[i]);
  }
  //Rect hull, old, new;
  //old.x1 = invader_army.x;
  //old.y1 = invader_army.y;
  //old.x2 = invader_army.x + invader_army.width;
  //old.y2 = invader_army.y + invader_army.height;
  //uint16_t new_left = old.x2 + invader_army.step;
  //if(new_left > LCD_W || new_left < 0) {
  //  invader_army.step = -invader_army.step;
  //  new.y1 = old.y1 - invader_army.drop;
  //  new.y2 = old.y2 - invader_army.drop;
  //} else {
  //  new.x1 = old.x1 + invader_army.step;
  //  new.x2 = old.x2 + invader_army.step;
  //  new.y1 = old.y1;
  //  new.y2 = old.y2;
  //}
  //compute_hull(old, new, &hull);

  //invader_army.x = new.x1;
  //invader_army.y = new.y1;
  //// Start the draw call
  //lcddev.select(1);
  //LCD_SetWindow(invader_army.x, invader_army.y, invader_army.x + invader_army.width - 1,
  //              invader_army.y + invader_army.height - 1);
  //LCD_WriteData16_Prepare();

  //// Draw the army
  //for (int army_y = INVADERS_HEIGHT - 1; army_y >= 0; army_y--) {
  //  for (int sprite_y = invader1_a_height - 1; sprite_y >= 0; sprite_y--) {
  //    for (int army_x = INVADERS_WIDTH - 1; army_x >= 0; army_x--) {
  //      uint8_t invader_type = invader_army.units[army_x + army_y * INVADERS_WIDTH];
  //      for (int sprite_x = invader1_a_width - 1; sprite_x >= 0; sprite_x--) {
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
  //LCD_WriteData16_End();
  //lcddev.select(0);

}
