#ifndef LISTA_PEDRAS_H
#define LISTA_PEDRAS_H
#include "init_sprites.h"

typedef struct nodo{
    int caindo, x, y; //att = se precisa atualizar ou não
    struct nodo *next;
}nodo;

nodo *inicializa_lista(void);
nodo *cria_nodo(int caindo, int x, int y);
nodo *insere_nodo(nodo **ini, nodo *input);
int deleta_nodo(nodo *ini, nodo *select);
void desenha_pedras(nodo *ini, t_sprites sprites);
int atualiza_pedras(nodo **ini, tile **area, t_sprites sprites);
void destroi_lista(nodo *ini);
nodo *busca_nodo(nodo* ini, int x, int y);
#endif