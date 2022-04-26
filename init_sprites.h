#ifndef INIT_SPRITES_H
#define INIT_SPRITES_H

#define branco al_map_rgb(255,255,255)
#define preto al_map_rgb(0,0,0)
#define bordo al_map_rgb(147,56,76)
#define aqua al_map_rgb(185,250,250)
#define rosa al_map_rgb(210,129,235)
#define verde al_map_rgb(110,207,106)
#define roxo al_map_rgb(79,73,214)
#define amareloClaro al_map_rgb(250,250,147)
#define marromClaro al_map_rgb(216,156,96)
#define marrom al_map_rgb(217,81,12)
#define cinza al_map_rgb(86,86,82)
#define lavanda al_map_rgb(165,155,252)

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
        *firefly[4],
        *butterfly[4],
        *prof;
}t_sprites;

t_sprites carrega_sprites(void);

#endif