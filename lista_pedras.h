#include "init_sprites.h"

#ifndef LISTA_PEDRAS_H
#define LISTA_PEDRAS_H

typedef struct nodo{
    int dir, att, x, y; //att = se precisa atualizar ou não
    struct nodo *next;
}nodo;

nodo *inicializa_lista(void);
void insere(nodo *ini, int dir, int att, int x, int y);
int deleta(nodo *ini, nodo *select);
void desenha_pedras(nodo *ini, t_sprites sprites);
#endif