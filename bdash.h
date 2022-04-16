#ifndef BDASH_H
#define BDASH_H

#define KEY_SEEN 1
#define KEY_RELEASED 2

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 352

#define LIN 23
#define COL 40

#define STILL 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

typedef struct level{
    int n_diams, n_rocks, time;
}level;

typedef struct jogador{
    int x, y;
    int vivo, score, dir;
}jogador;

typedef struct tile{
    enum tipo{Empty = 0, Wall, Dirt, Player, Rock, Exit, Brick, Diamond}tipo;
    int movendo;
}tile;

typedef struct jogo{
    int n_level, d_restantes, t_restante; 
}jogo;

ALLEGRO_DISPLAY *inicializa_tela(int w, int h);
ALLEGRO_TRANSFORM setup_transform(ALLEGRO_DISPLAY *disp);

#endif