#ifndef SOUND_H
#define SOUND_H
#include "notes.h"
#include "stm32f0xx.h"
#include <math.h>
#include <stdint.h>

#define SAMPLES 50
extern uint16_t wavetable[SAMPLES];

extern uint16_t melody_idx;
extern uint8_t melody_select;

extern uint16_t melody1[];
extern uint16_t melody2[];

// How long to hold each note for in ms
extern const uint16_t noteDurations1[];
extern const uint16_t noteDurations2[];

extern const uint16_t melody1_len;
extern const uint16_t melody2_len;

void setup_music();
void start_music();
void pause_music();
void resume_music();

#endif
