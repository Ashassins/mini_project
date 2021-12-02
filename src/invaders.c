#include "invaders.h"

void init_invaders() {
  uint8_t type = 1;
  for(int i = 0; i < INVADERS_HEIGHT; i++) {
    for(int j = 0; j < INVADERS_WIDTH; j++) {
      if (i == 0) {
        type = 1;
      } else if (i <= 2) {
        type = 2;
      } else {
        type = 3;
      }
      invaders.units[j + i * INVADERS_WIDTH] = type;
    }
  }
  invaders.x = 220;
  invaders.y = 280;
}

// TODO
void draw_invaders(){

}

// TODO
void update_invaders() {

}
