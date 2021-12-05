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
#define BUNKER_FULL_HEALTH 4
//Bunker bunker;
BunkerArray bunkers;
Sprite clean_bunker;


//*/
int _n_bunkers = sizeof(bunkers.bunkerArray) / sizeof(bunkers.cleanBunker);
// this needs to be a clean bunker
void init_4_bunkers() { // these x's, y's can be changed later
    // initialize bunkers
    for (int i = 0; i < BUNKER_COUNT; i++) {
        bunkers.bunkerArray[i].id = i;
        bunkers.bunkerArray[i].hp = BUNKER_FULL_HEALTH;
        bunkers.bunkerArray[i].x = 10 + 30 * i;
        bunkers.bunkerArray[i].y = 45;
        //bunkers.bunkerArray[i].nextBunkerStage. = bunker_dead;//bunker_lightly_damaged;
        //bunkers.bunkerArray[i].cleanBunker = bunker_clean;
        init_sprite(bunkers.bunkerArray[i].x,bunkers.bunkerArray[i].y ,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &(bunkers.bunkerArray[i].cleanBunker));
        init_sprite(bunkers.bunkerArray[i].x,bunkers.bunkerArray[i].y ,bunker_clean_width, bunker_clean_height, (uint16_t*)bunker_clean, (uint16_t*)bunker_clean, &(bunkers.bunkerArray[i].nextBunkerStage));
        draw_sprite(&(bunkers.bunkerArray[i].cleanBunker));
    }
}

void damage_bunker(Bunker bunker) {
    int initial_hp = bunker.hp;
    if (bunker.hp != 0) {
        bunker.hp -= 1;
    }
    int hp = bunker.hp;
    if (bunker.hp == 4){
        bunker.nextBunkerStage.sprite_data = &bunker_clean;
    } else if (bunker.hp == 3){
        bunker.nextBunkerStage.sprite_data = &bunker_dead;//bunker_lightly_damaged;
    } else if (bunker.hp == 2){
        bunker.nextBunkerStage.sprite_data = &bunker_dead;//bunker_moderately_damaged;
    } else if (bunker.hp == 1){
        bunker.nextBunkerStage.sprite_data = &bunker_dead;//bunker_heavily_damaged;
    } else if (bunker.hp == 0){
        bunker.nextBunkerStage.sprite_data = &bunker_dead; 
    } //bunker.nextBunkerStage = sprite;
}

uint8_t bunker_coll(Sprite *shot) {
    for (int i = 0; i < BUNKER_COUNT; i++) {
        if (sprite_coll(shot, &(bunkers.bunkerArray[i].cleanBunker)) && (bunkers.bunkerArray[i].hp > 0) && (bunkers.bunkerArray[i].hp <= 4)) {
            update_bunker(i);
            return 1;
        }
    }
    return 0;
}

void update_bunker(uint8_t bunkerId) {//, Sprite sprite) {
    //if (i == bunkers.bunkerArray[i].id) {
        //if (collision) {
        damage_bunker(bunkers.bunkerArray[bunkerId]);
        redraw_bunker(&(bunkers.bunkerArray[bunkerId].nextBunkerStage));
        //}
        /*
        else {
            redraw_bunker(i, &(bunkers.bunkerArray[i].cleanBunker));
        }
        // redraw specific bunker*/
    
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

void redraw_bunker(Sprite *nextBunker) {
    prepDraw();
    draw_sprite(&nextBunker);
    endDraw();
    //bunkers.bunkerArray[i].cleanBunker = bunkers.bunkerArray[i].nextBunkerStage;
    //bunkers.bunkerArray[i].nextBunkerStage = null;
}


