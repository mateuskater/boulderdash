#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

#include "init_sprites.h"
#include "som.h"

#define KEY_SEEN 1
#define KEY_RELEASED 2

#define LIN 23
#define COL 40

#define STILL 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define TEMPO_LIMITE 150
#define OFF 16 // offset para dar espaço ao HUD

typedef struct nodo{ // implementação de lista encadeada
    int caindo, x, y; //att = se precisa atualizar ou não
    int dir; // direcao, para os inimigos
    struct nodo *next; // ponteiro para o proximo nodo da lista
}nodo;

typedef struct jogador{
    int x, y; // coordenadas do jogador
    int score, dir; // pontuação e direção que ele está indo
    int vivo, vidas; //se ele está vivo ou quantidade de vidas
}jogador;

typedef struct tile{
    enum{Empty = 0, Wall, Dirt, Player, Rock, SaidaFechada, SaidaAberta, Enemy, Brick, Diamond}tipo;
}tile;

typedef struct jogo{
    int n_level, d_restantes, n_diams, t_restante;
    int saida_x, saida_y; // coordenadas da saida do level
    int reset; //variavel para sair do loop, reiniciar o level ou trocar de level
}jogo;

tile **aloca_area(int lin, int col);
void inicializa_jogo(tile **area, jogador *player, nodo **pedras, nodo **diamantes, nodo **butterflies, nodo **fireflies, jogo *jogo);
int colisao(tile **area, int direcao, jogador player);
void move_player(tile **area, jogador *player, int dir);
int empurra(tile **area, jogador *player, nodo *pedras, int dir);
void coleta_diamante(tile **area, jogador *player, jogo *jogo, t_sons sons, nodo **diamantes);
void desenha_mapa(tile **area, t_sprites sprites);
void testa_init(bool test, const char *objeto);
int explode(tile **area, t_sprites sprites, int x, int y, int frame);
void abre_saida(tile **area, jogo jogo);
void gera_tela_help(ALLEGRO_BITMAP **help);

#endif