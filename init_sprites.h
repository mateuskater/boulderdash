#ifndef INIT_SPRITES_H
#define INIT_SPRITES_H

typedef struct sprites{
    ALLEGRO_BITMAP* player[7], *player_left[7],
        *player_right[7],
        *wall,
        *brick,
        *diamond[8],
        *morte[5],
        *dirt,
        *rock,
        *exit,
        *prof;
}t_sprites;

t_sprites carrega_sprites(void);

#endif