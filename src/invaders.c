#include "invaders.h"
#include "lcd.h"

Invaders invaders;
int _n_unit_type =
    sizeof(invaders.unit_sprites) / sizeof(invaders.unit_sprites[0]);

void init_invaders() {
  uint8_t type = 1;
  for (int i = 0; i < INVADERS_HEIGHT; i++) {
    for (int j = 0; j < INVADERS_WIDTH; j++) {
      if (i == 0) {
        type = 3;
      } else if (i <= 2) {
        type = 2;
      } else {
        type = 1;
      }
      invaders.units[j + i * INVADERS_WIDTH] = type;
    }
  }
  invaders.x = 0;
  invaders.y = 230;
  invaders.height = INVADERS_HEIGHT * invader1_a_height;
  invaders.width = INVADERS_WIDTH * invader1_a_width;
  invaders.unit_sprites[0] = (uint16_t *)invader1_a;
  invaders.unit_sprites[1] = (uint16_t *)invader2_a;
  invaders.unit_sprites[2] = (uint16_t *)invader3_a;
  invaders.unit_sprites[3] = (uint16_t *)invader_explode;

  invaders.unit_sprite_switch_keys[0] =
      ((uint32_t)invader1_a) ^ ((uint32_t)invader1_b);
  invaders.unit_sprite_switch_keys[1] =
      ((uint32_t)invader2_a) ^ ((uint32_t)invader2_b);
  invaders.unit_sprite_switch_keys[2] =
      ((uint32_t)invader3_a) ^ ((uint32_t)invader3_b);
  // This one is zero because we only have one frame for the explosion
  invaders.unit_sprite_switch_keys[3] = 0;
}

void draw_invaders() {
  lcddev.select(1);
  LCD_SetWindow(invaders.x, invaders.y, invaders.x + invaders.width - 1,
                invaders.y + invaders.height + (INVADERS_HEIGHT * 2) - 1);
  LCD_WriteData16_Prepare();

  // Draw the army
  for (int army_y = INVADERS_HEIGHT - 1; army_y >= 0; army_y--) {
    for (int sprite_y = invader1_a_height - 1; sprite_y >= 0; sprite_y--) {
      for (int army_x = INVADERS_WIDTH - 1; army_x >= 0; army_x--) {
        uint8_t invader_type = invaders.units[army_x + army_y * INVADERS_WIDTH];
        for (int sprite_x = invader1_a_width - 1; sprite_x >= 0; sprite_x--) {
          switch (invader_type) {
          case 1:
            LCD_WriteData16(
                invaders.unit_sprites[0][sprite_y + sprite_x * invader1_a_height]);
            break;
          case 2:
            LCD_WriteData16(
                invaders.unit_sprites[1][sprite_y + sprite_x * invader1_a_height]);
            break;
          case 3:
            LCD_WriteData16(
                invaders.unit_sprites[2][sprite_y + sprite_x * invader1_a_height]);
            break;
          default:
            LCD_WriteData16(0);
          }
        }
      }
    }
    // Row spacing
    for (int x = (invader1_a_width * INVADERS_WIDTH * 2) - 1; x >= 0; x--) {
      LCD_WriteData16(0);
    }
  }
  LCD_WriteData16_End();
  lcddev.select(0);
}

// TODO Move the invaders by step, when they hit the edge of the screen make step negative
// TODO Clear the region where the invaders were but no longer are
void update_invaders() {
  for (int i = 0; i < _n_unit_type; i++) {
    invaders.unit_sprites[i] =
        (uint16_t *)(((uint32_t)invaders.unit_sprites[i]) ^
                     invaders.unit_sprite_switch_keys[i]);
  }
}
