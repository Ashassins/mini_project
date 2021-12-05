#include "bunker.h"
#include "lcd.h"
#include "util.h"
#include "sprites.h"
#include "sprite_data.h"
//#include "sprite_data.c"
#define BUNKER_WINDOW_TOP_X 240 //LCD_W
#define BUNKER_WINDOW_TOP_Y 121
#define BUNKER_WINDOW_BOTTOM_X 0
#define BUNKER_WINDOW_BOTTOM_Y 45

Bunker bunker;
BunkerArray bunkers;
Sprite clean_bunker;


Sprite bunker1;
Sprite bunker2;
Sprite bunker3;
Sprite bunker4;
//*/
int _n_bunkers = sizeof(bunkers.bunkerArray) / sizeof(bunkers.cleanBunker);
// this needs to be a clean bunker
void init_4_bunkers() { // these x's, y's can be changed later
    init_sprite(10,45,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &bunker1);
    init_sprite(30,45,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &bunker2);
    init_sprite(50,45,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &bunker3);
    init_sprite(70,45,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &bunker4);
    draw_sprite(&bunker1);
    draw_sprite(&bunker2);
    draw_sprite(&bunker3);
    draw_sprite(&bunker4);
}

void damage_bunker(Bunker bunker) {
    bunker.hp -= 1;
    if (bunker.hp == 4){
        bunker.nextBunkerStage->sprite_data = bunker_clean;
    } else if (bunker.hp == 3){
        bunker.nextBunkerStage->sprite_data = bunker_dead;//bunker_lightly_damaged;
    } else if (bunker.hp == 2){
        bunker.nextBunkerStage->sprite_data = bunker_dead;//bunker_moderately_damaged;
    } else if (bunker.hp == 1){
        bunker.nextBunkerStage->sprite_data = bunker_dead;//bunker_heavily_damaged;
    } else if (bunker.hp == 0){
        bunker.nextBunkerStage->sprite_data = bunker_dead; 
        // clear bbox as well 
    } //bunker.nextBunkerStage = sprite;
}

void update_bunker(uint8_t bunkerId, Sprite sprite) {
    for (int i = 0; i < _n_bunkers; i++) {
        if (bunkerId == bunkers.bunkerArray[i].id) {
            damage_bunker(bunkers.bunkerArray[i]);
            redraw_bunker(bunkerId, bunkers.bunkerArray[i].nextBunkerStage);
            // redraw specific bunker
        }
    }
}

void prepDraw() {
    lcddev.select(1);
    LCD_SetWindow(BUNKER_WINDOW_TOP_X, BUNKER_WINDOW_TOP_Y, BUNKER_WINDOW_BOTTOM_X, BUNKER_WINDOW_BOTTOM_Y);
    LCD_WriteData16_Prepare();
}

void endDraw() {
    LCD_WriteData16_End();
    lcddev.select(0);
}

void redraw_bunker(uint8_t bunkerId, Sprite *nextBunker) {
    prepDraw();
    draw_sprite(nextBunker);
    endDraw();
}

uint8_t bunker_coll(Sprite *shot) {
    for (int i = 0; i < BUNKER_COUNT; i++) {
        if (sprite_coll(shot, &(bunkers.bunkerArray[i])) && bunkers.bunkerArray[i].hp != 0) {
            //damage_bunker();
            return 1;
        }
    }
    return 0;
}


