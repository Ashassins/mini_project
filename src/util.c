#include "util.h"

// Return the rect that contains a and b
void compute_hull(Rect a, Rect b, Rect *rv) {
  rv->x1 = umin(a.x1, b.x1);
  rv->x2 = umax(a.x2, b.x2);
  rv->y1 = umin(a.y1, b.y1);
  rv->y2 = umax(a.y2, b.y2);
}

uint8_t overlap(Rect a, Rect b) {
  // Kinda ripped from here
  // https://stackoverflow.com/questions/9324339/how-much-do-two-rectangles-overlap/9325084
  int32_t dx = a.x1 - b.x2;
  int32_t dy = a.y1 - b.y2;
  // Check if the sign is the same
  return ~((dx ^ dy) >> 31) & 1;
}


uint8_t contains(uint16_t x, uint16_t y, Rect r) {
  return x >= r.x1 && x <= r.x2 && y >= r.y1 && y <= r.y2;
}

uint32_t umax(uint32_t a, uint32_t b) {
  return a > b ? a : b;
}

uint32_t umin(uint32_t a, uint32_t b) {
  return a < b ? a : b;
}
