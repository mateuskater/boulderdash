#ifndef BDASH_H
#define BDASH_H

#include "init_sprites.h"

#define KEY_SEEN 1
#define KEY_RELEASED 2

#define LIN 23
#define COL 40

#define STILL 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define ESQUERDA -1
#define DIREITA 1

#define TEMPO_LIMITE 127
#define OFF 16 // offset para dar espaço ao HUD

typedef struct jogador{
    int x, y;
    int score, dir;
}jogador;

typedef struct tile{
    enum{Empty = 0, Wall, Dirt, Player, Rock, SaidaFechada, SaidaAberta, Brick, Diamond}tipo;
}tile;

typedef struct jogo{
    int n_level, d_restantes, n_diams, t_restante;
    int saida_x, saida_y; 
}jogo;

typedef struct nodo{
    int caindo, x, y; //att = se precisa atualizar ou não
    struct nodo *next;
    struct nodo *prev;
}nodo;

tile **aloca_area(int lin, int col);
void inicializa_jogo(tile **area, jogador *player, nodo **pedras, nodo **diamantes, jogo *jogo);
int colisao(tile **area, int direcao, jogador player);
void move_player(tile **area, jogador *player, int dir);
int empurra(tile **area, jogador *player, nodo *pedras, int dir);
void desenha_mapa(tile **area, t_sprites sprites);
void testa_init(bool test, const char *objeto);
int morte(jogador player, t_sprites sprites, int frame);

#endif