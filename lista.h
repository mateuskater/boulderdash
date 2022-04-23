#ifndef LISTA_H
#define LISTA_H

#include "game.h"
#include "init_sprites.h"

nodo *inicializa_lista(void);
nodo *cria_nodo(int caindo, int x, int y);
nodo *insere_nodo(nodo **ini, nodo *input);
nodo *busca_nodo(nodo *ini, int x, int y);
int deleta_nodo(nodo **ini, nodo *select);
int conta_nodos(nodo *ini);
void desenha_pedras(nodo *ini, t_sprites sprites);
void desenha_diamantes(nodo *ini, t_sprites sprites, int frame);
int rola(tile **area, nodo *atual, int dir);
int atualiza_objetos(nodo **ini, tile **area, t_sprites sprites, char item);
void destroi_lista(nodo **ini);

#endif