#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "bdash.h"
#include "lista_pedras.h"
#include "init_sprites.h"

nodo *inicializa_lista(){
    nodo *ini = NULL;
    return ini;
}

void insere(nodo *ini, int dir, int att, int x, int y){
    nodo *new = malloc(sizeof(nodo));
    if(ini == NULL)
        ini = new;
    new->dir = dir;
    new->att = att;
    new->x = x;
    new->y = y;
    new->next = ini;
    ini = new;
}

int deleta(nodo *ini, nodo *select){
    nodo *aux = ini, *temp = NULL;
    while(aux->next != select){
        aux->next = aux;
        if(aux->next == NULL)
            return 1;
    }
    temp = aux->next;
    aux->next = aux->next->next;
    free(temp);
    return 0;
}

void desenha_pedras(nodo *ini, t_sprites sprites){
    nodo *aux = ini;
    while(aux != NULL){
        al_draw_bitmap(sprites.rock, aux->x, aux->y, 0);
    }
}

void atualiza_pedras(nodo *ini, tile **area, t_sprites sprites, int x, int y){
    nodo *aux = ini;
    while(aux->y != y && aux->x != x)
        aux = aux->next;
    aux->att = 1;
    if (area[aux->y+1][aux->x].tipo == Empty)
        aux->y++;
}