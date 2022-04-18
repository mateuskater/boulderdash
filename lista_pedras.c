#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "bdash.h"
#include "lista_pedras.h"
#include "init_sprites.h"

nodo *inicializa_lista(){
    nodo *ini = malloc(sizeof(nodo));
    return ini;
}

nodo *cria_nodo(int caindo, int x, int y){
    nodo *new = malloc(sizeof(nodo));
    new->caindo = caindo;
    new->x = x;
    new->y = y;
    new->next = NULL;
    return new;
}

nodo *insere_nodo(nodo **ini, nodo *input){
    input->next = *ini;
    *ini = input;
    return input;
}

int deleta_nodo(nodo *ini, nodo *select){
    nodo *aux = ini, *temp = NULL;
    while(aux->next != select){
        aux->next = aux;
        if(aux == NULL)
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
        al_draw_bitmap(sprites.rock, aux->x*16, aux->y*16, 0);
        aux = aux->next;
    }
}

nodo *busca_nodo(nodo* ini, int x, int y){
    nodo *aux = ini;

    while(aux->x != x && aux->y != y && aux->next != NULL)
        aux = aux->next;
    if(aux->x == x && aux->y == y)
        return aux;
    else
        return NULL;
}

int atualiza_pedras(nodo **ini, tile **area, t_sprites sprites){
    nodo *aux = *ini;

    while(aux->next != NULL){
        if (area[aux->y+1][aux->x].tipo == Player && aux->caindo == 1)
            return 1;
        if (area[aux->y+1][aux->x].tipo == Empty){
            aux->caindo = 1;
            area[aux->y+1][aux->x].tipo = Rock;
            area[aux->y][aux->x].tipo = Empty;
            aux->y++;
        }else if( (area[aux->y+1][aux->x].tipo == Rock || area[aux->y+1][aux->x].tipo == Brick) && area[aux->y][aux->x+1].tipo == Empty && area[aux->y+1][aux->x+1].tipo == Empty){
            area[aux->y][aux->x+1].tipo = Rock; // caso direita e diagonal inferior direita estejam livres
            area[aux->y][aux->x].tipo = Empty; // pedra desliza para a direita
            aux->x++;
        }else if( (area[aux->y+1][aux->x].tipo == Rock || area[aux->y+1][aux->x].tipo == Brick) && area[aux->y][aux->x-1].tipo == Empty && area[aux->y+1][aux->x-1].tipo == Empty){
            area[aux->y][aux->x-1].tipo = Rock; //caso esquerda e diagonal inferior esquerda estejam livres
            area[aux->y][aux->x].tipo = Empty; // pedra desliza para a esquerda
            aux->x--;
        }else aux->caindo = 0;
        aux = aux->next;
    }
    return 0;
}

void destroi_lista(nodo *ini){
    nodo *aux = ini, *temp = ini;
    while(aux != NULL){
        aux = aux->next;
        free(temp);
        temp = aux;
    }
}