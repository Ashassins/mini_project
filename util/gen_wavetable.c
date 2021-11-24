#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main() {
  int SAMPLES = 50;
  printf("const uint16_t wavetable[SAMPLES] = {\n\t");
  for(int i = 0; i < SAMPLES; i++) {
    printf("%hu, ", (uint16_t)(2048 + 1952 * (1 - fabs((float)i / SAMPLES - 0.5) * 4)));
    if (i % 5 == 4) {
      printf("\n\t");
    }
  }
  printf("};\n");
}
