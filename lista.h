#ifndef LISTA_H
#define LISTA_H
#include "init_sprites.h"

typedef struct nodo{
    int caindo, x, y; //att = se precisa atualizar ou não
    struct nodo *next;
    struct nodo *prev;
}nodo;

nodo *inicializa_lista(void);
nodo *cria_nodo(int caindo, int x, int y);
nodo *insere_nodo(nodo **ini, nodo *input);
int deleta_nodo(nodo *ini, nodo *select);
void desenha_pedras(nodo *ini, t_sprites sprites);
void desenha_diamantes(nodo *ini, t_sprites sprites, int frame);
int atualiza_objetos(nodo **ini, tile **area, t_sprites sprites, char item);
void destroi_lista(nodo *ini);
nodo *busca_nodo(nodo* ini, int x, int y);
#endif