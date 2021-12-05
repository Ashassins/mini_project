#ifndef BUNKER_H
#define BUNKER_H

#include "lcd.h"
#include "sprite_data.h"
#include "sprites.h" 

#define BUNKER_COUNT 4
#define BUNKER_WIDTH 76
#define BUNKER_HEIGHT 5

typedef struct {
    uint8_t id; // each bunker has its own ID
    uint8_t hp; // total num hits bunker can take
    //uint16_t width, height; // this will update as the bunker is destroyed
    uint16_t x, y;
    Sprite *nextBunkerStage;
    //Rect bboxLeft;
    //Rect bboxCenter;
    //Rect bboxRight;
    //uint16_t x, y;  // bottom right corner
    //\uint16_t *sprite_data; 
    //uint32_t sprite_swap_key; // still the same evil bit fuckery lol
} Bunker;

typedef struct {
    Rect bboxTotal;
    Bunker bunkerArray[BUNKER_COUNT];
    uint16_t cleanBunker;
    //Sprite bunkerSpriteArray[BUNKER_COUNT];
    //Rect *bboxArray;
    //uint16_t *bunker_array[BUNKER_COUNT];
} BunkerArray;

void init_4_bunkers();
void init_bunker(uint16_t x, uint16_t y);
void damage_bunkers(Bunker bunker);
void update_bunkers(uint8_t bunkerId);
void redraw_bunker(uint8_t bunkerId, Sprite *nextBunker);
void endDraw(); // just for my peace of mind
void prepDraw();
// [deprecated] 3 rectangles ish
/* [deprecated]
        |***|
    ****|***|****
    ****|   |****
*/
#endif