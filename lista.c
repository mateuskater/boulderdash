#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "game.h"
#include "lista.h"
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

nodo *busca_nodo(nodo *ini, int x, int y){
    nodo *aux = ini;

    while(aux->x != x && aux->y != y && aux->next != NULL)
        aux = aux->next;
    if(aux->x == x && aux->y == y)
        return aux;
    else
        return NULL;
}

int deleta_nodo(nodo **ini, nodo *select){
    nodo *aux = *ini, *temp = NULL;
    while(aux->next != select){
        aux = aux->next;
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
        al_draw_bitmap(sprites.rock, aux->x*16, aux->y*16 + OFF, 0);
        aux = aux->next;
    }
}

void desenha_diamantes(nodo *ini, t_sprites sprites, int frame){
    nodo *aux = ini;
    while(aux != NULL){
        al_draw_bitmap(sprites.diamond[frame], aux->x*16, aux->y*16 + OFF, 0);
        aux = aux->next;
    }
}

int rola(tile **area, nodo *atual, int dir){

    if((area[atual->y+1][atual->x].tipo == Rock || area[atual->y+1][atual->x].tipo == Brick || area[atual->y+1][atual->x].tipo == Diamond) 
                    && area[atual->y][atual->x+dir].tipo == Empty // caso tenha brick, rock ou diamond em baixo
                    && area[atual->y+1][atual->x+dir].tipo == Empty)
        return 1;
    return 0;
}

int atualiza_objetos(nodo **ini, tile **area, t_sprites sprites, char item){
    nodo *aux = *ini;
    int objeto;

    if(item == 'd')
        objeto = Diamond;
    else if(item == 'r')
        objeto = Rock;

    while(aux->next != NULL){
        // if (area[aux->y+1][aux->x].tipo == Player && aux->caindo == 1)
            // return 1;
        if (area[aux->y+1][aux->x].tipo == Empty){
            aux->caindo = 1;
            area[aux->y+1][aux->x].tipo = objeto;
            area[aux->y][aux->x].tipo = Empty;
            aux->y++;
        }else if(rola(area, aux, DIREITA)){
            area[aux->y][aux->x+1].tipo = objeto; // caso direita e diagonal inferior direita estejam livres
            area[aux->y][aux->x].tipo = Empty; // pedra desliza para a direita
            aux->x++;
        }else if(rola(area, aux, ESQUERDA)){
            area[aux->y][aux->x-1].tipo = objeto; //caso esquerda e diagonal inferior esquerda estejam livres
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