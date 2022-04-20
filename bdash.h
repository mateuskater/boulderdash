#ifndef BDASH_H
#define BDASH_H

#define KEY_SEEN 1
#define KEY_RELEASED 2

#define LIN 24
#define COL 40

#define STILL 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define ESQUERDA -1
#define DIREITA 1

#define OFF 16 // offset para dar espaço ao HUD
typedef struct level{
    int n_diams, n_rocks, time;
}level;

typedef struct jogador{
    int x, y;
    int score, dir;
}jogador;

typedef struct tile{
    enum{Empty = 0, Wall, Dirt, Player, Rock, Exit, Brick, Diamond}tipo;
    int movendo;
}tile;

typedef struct jogo{
    int n_level, d_restantes, t_restante; 
}jogo;

#endif