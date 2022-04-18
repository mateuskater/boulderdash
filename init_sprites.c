#include <stdio.h>
#include <allegro5/allegro.h>
#include "bdash.h"
#include "init_sprites.h"


t_sprites carrega_sprites(void){
    int i, j;
    t_sprites sprites;
    ALLEGRO_BITMAP* sheet = al_load_bitmap("./resources/spritesheet.png");
    
    for (i = 0; i < 7; i++){
        sprites.player[i] = al_create_sub_bitmap(sheet, i*16, 0, 16, 16);
        sprites.player_left[i] = al_create_sub_bitmap(sheet, i*16, 16, 16, 16);
        sprites.player_right[i] = al_create_sub_bitmap(sheet, i*16, 32, 16, 16);
    }
    
    sprites.wall = al_create_sub_bitmap(sheet, 0, 48, 16, 16);
    sprites.brick = al_create_sub_bitmap(sheet, 32, 48, 16, 16);

    for(i = 0; i < 2; i++)
        for(j = 0; j < 4; j++)
            sprites.diamond[i+j] = al_create_sub_bitmap(sheet, i*16, 64+j*16, 16, 16);
    
    sprites.morte[0] = al_create_sub_bitmap(sheet, 112, 64, 16, 16);
    sprites.morte[1] = al_create_sub_bitmap(sheet, 112, 80, 16, 16);
    sprites.morte[2] = al_create_sub_bitmap(sheet, 112, 96, 16, 16);
    sprites.morte[3] = al_create_sub_bitmap(sheet, 112, 80, 16, 16);
    sprites.morte[4] = al_create_sub_bitmap(sheet, 112, 64, 16, 16);

    sprites.dirt = al_create_sub_bitmap(sheet, 48, 48, 16, 16);
    sprites.rock = al_create_sub_bitmap(sheet, 80, 48, 16, 16);

    for(i = 0; i < 7; i++){
        al_convert_mask_to_alpha(sprites.player[i], al_map_rgb(18, 23, 18));
        al_convert_mask_to_alpha(sprites.player_left[i], al_map_rgb(18, 23, 18));
        al_convert_mask_to_alpha(sprites.player_right[i], al_map_rgb(18, 23, 18));
    }
    al_convert_mask_to_alpha(sprites.rock, al_map_rgb(18, 23, 18));

    return sprites;
}