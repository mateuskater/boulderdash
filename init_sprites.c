#include <stdio.h>
#include <allegro5/allegro.h>
#include "game.h"
#include "init_sprites.h"

t_sprites carrega_sprites(void){
    int i;
    t_sprites sprites;
    ALLEGRO_BITMAP* sheet = al_load_bitmap("./resources/spritesheet.png");
    
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);
    
    for (i = 0; i < 7; i++){
        sprites.player[i] = al_create_sub_bitmap(sheet, i*16, 0, 16, 16);
        sprites.player_left[i] = al_create_sub_bitmap(sheet, i*16, 16, 16, 16);
        sprites.player_right[i] = al_create_sub_bitmap(sheet, i*16, 32, 16, 16);
    }
    
    sprites.wall = al_create_sub_bitmap(sheet, 0, 48, 16, 16);
    sprites.brick = al_create_sub_bitmap(sheet, 32, 48, 16, 16);

    sprites.diamond[0] = al_create_sub_bitmap(sheet, 0, 64, 16, 16);
    sprites.diamond[1] = al_create_sub_bitmap(sheet, 0, 80, 16, 16);
    sprites.diamond[2] = al_create_sub_bitmap(sheet, 0, 96, 16, 16);
    sprites.diamond[3] = al_create_sub_bitmap(sheet, 0, 112, 16, 16);
    sprites.diamond[4] = al_create_sub_bitmap(sheet, 16, 64, 16, 16);
    sprites.diamond[5] = al_create_sub_bitmap(sheet, 16, 80, 16, 16);
    sprites.diamond[6] = al_create_sub_bitmap(sheet, 16, 96, 16, 16);
    sprites.diamond[7] = al_create_sub_bitmap(sheet, 16, 112, 16, 16);
    
    sprites.explosao[0] = al_create_sub_bitmap(sheet, 112, 64, 16, 16);
    sprites.explosao[1] = al_create_sub_bitmap(sheet, 112, 80, 16, 16);
    sprites.explosao[2] = al_create_sub_bitmap(sheet, 112, 96, 16, 16);
    sprites.explosao[3] = al_create_sub_bitmap(sheet, 112, 80, 16, 16);
    sprites.explosao[4] = al_create_sub_bitmap(sheet, 112, 64, 16, 16);

    sprites.dirt = al_create_sub_bitmap(sheet, 48, 48, 16, 16);
    sprites.rock = al_create_sub_bitmap(sheet, 80, 48, 16, 16);

    sprites.exit = al_create_sub_bitmap(sheet, 16, 48, 16, 16);

    for(i = 0; i < 4; i++){
        sprites.firefly[i] = al_create_sub_bitmap(sheet, 80, 64+(i*16), 16, 16); 
        sprites.butterfly[i] = al_create_sub_bitmap(sheet, 96, 64+(i*16), 16, 16);
    }

    sprites.prof = al_load_bitmap("./resources/David-Menotti.png");

    return sprites;
}