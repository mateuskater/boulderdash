#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "game.h"
#include "lista.h"
#include "init_sprites.h"

nodo *inicializa_lista(){ // inicializa uma lista encadeada
    nodo *ini = malloc(sizeof(nodo));
    ini->next = NULL;
    return ini;
}

nodo *cria_nodo(int caindo, int x, int y){ // cria um nodo e o retorna
    nodo *new = malloc(sizeof(nodo));
    if(!new) // testa malloc
        exit(1);
    new->caindo = caindo;
    new->x = x;
    new->y = y;
    new->next = NULL;
    return new;
}

nodo *insere_nodo(nodo **ini, nodo *input){ // insere na lista um nodo passado
    input->next = *ini;
    *ini = input;
    return input;
}

nodo *busca_nodo(nodo *ini, int x, int y){ // busca um nodo pelas coordenadas e o retorna
    nodo *aux = ini;
    if (ini == NULL)
        return NULL; 
    while( (aux->x != x || aux->y != y) && aux->next != NULL)
        aux = aux->next;
    if(aux->x == x && aux->y == y)
        return aux; // caso tenha encontrado
    else
        return NULL; // caso não tenha encontrado
}

int deleta_nodo(nodo **ini, nodo *select){ // deleta um nodo fornecido como parametro
    nodo *aux = *ini, *temp = NULL;
    if(*ini == select){ // caso a lista tenha apenas um elemento
        temp = select;
        *ini = NULL;
        free(temp);
        return 0;
    }
    while(aux->next != select){
        aux = aux->next;
        if(aux->next == NULL)
            return 1;
    }
    temp = aux->next;
    aux->next = aux->next->next;
    free(temp);
    return 0;
}

int conta_nodos(nodo *ini){ // retorna a quantidade de nodos na lista
    nodo *aux = ini;
    int i = 0;
    while(aux->next != NULL){
        aux = aux->next;
        i++;
    }
    return i;
}

void desenha_pedras(nodo *ini, t_sprites sprites){ // desenha todas as pedras da lista encadeada na tela
    nodo *aux = ini;
    while(aux != NULL){
        // al_draw_scaled_bitmap(sprites.rock, 0, 0, 166, 240, aux->x*16, aux->y*16 + OFF, 16, 16, 0);
        al_draw_bitmap(sprites.rock, aux->x*16, aux->y*16 + OFF, 0);
        aux = aux->next;
    }
}

void desenha_diamantes(nodo *ini, t_sprites sprites, int frame){ // desenha todos os diamantes da lista encadeada na tela
    nodo *aux = ini;
    while(aux != NULL){
        al_draw_bitmap(sprites.diamond[frame], aux->x*16, aux->y*16 + OFF, 0);
        aux = aux->next;
    }
}

void desenha_butterflies(nodo *ini, t_sprites sprites, int frame){ // desenha todas as butterflies da lista encadeada na tela
    nodo *aux = ini;
    while(aux != NULL){
        al_draw_bitmap(sprites.butterfly[frame], aux->x*16, aux->y*16 + OFF, 0);
        aux = aux->next;
    }
}

void desenha_fireflies(nodo *ini, t_sprites sprites, int frame){ // desenha todas as fireflies da lista encadeada na tela
    nodo *aux = ini;
    while(aux != NULL){
        al_draw_bitmap(sprites.firefly[frame], aux->x*16, aux->y*16 + OFF, 0);
        aux = aux->next;
    }
}

void desenha_menotti(nodo *ini, t_sprites sprites){ // desenha o prof
    nodo *aux = ini;
    while(aux != NULL){
        al_draw_scaled_bitmap(sprites.prof, 0, 0, 166, 240, aux->x*16, aux->y*16 + OFF, 16, 16, 0);
        aux = aux->next;
    }
}

int rola(tile **area, nodo *atual, int dir){ // verifica se o objeto deve rolar
    if((area[atual->y+1][atual->x].tipo == Rock || area[atual->y+1][atual->x].tipo == Brick || area[atual->y+1][atual->x].tipo == Diamond) 
                && area[atual->y][atual->x+dir].tipo == Empty // caso tenha brick, rock ou diamond em baixo
                && area[atual->y+1][atual->x+dir].tipo == Empty)
        return 1; // rola
    return 0; // não rola
}

int atualiza_objetos(nodo **ini, tile **area, t_sprites sprites, char item){
    nodo *aux = *ini;
    int objeto;

    if(item == 'd') //       aqui é passado um char para saber qual
        objeto = Diamond; // objeto será atualizado, diamante ou pedra 
    else if(item == 'r')
        objeto = Rock;

    while(aux->next != NULL){
        if (area[aux->y+1][aux->x].tipo == Player && aux->caindo == 1)
            return 1; // retorna 1 se mata o player
        if (area[aux->y+1][aux->x].tipo == Empty){ // se o espaço de baixo for vazio
            aux->caindo = 1; // atualiza valor caindo para positivo
            area[aux->y+1][aux->x].tipo = objeto; // o espaço de baixo vira o objeto em questão, pedra ou diamante
            area[aux->y][aux->x].tipo = Empty; 
            aux->y++;
        }else if(rola(area, aux, RIGHT)){
            area[aux->y][aux->x+1].tipo = objeto; // caso direita e diagonal inferior direita estejam livres
            area[aux->y][aux->x].tipo = Empty; // pedra desliza para a direita
            aux->x++;
        }else if(rola(area, aux, LEFT)){
            area[aux->y][aux->x-1].tipo = objeto; //caso esquerda e diagonal inferior esquerda estejam livres
            area[aux->y][aux->x].tipo = Empty; // pedra desliza para a esquerda
            aux->x--;
        }else aux->caindo = 0; // se não cair nesses casos, pedra não está caindo
        aux = aux->next;
    }
    return 0;
}

int atualiza_fireflies(nodo **bichos, tile **area, t_sprites sprites, jogador *player){
    nodo *aux = *bichos, *temp;

    while(aux->next != NULL){  
        if(aux->dir == UP)      
            switch(area[aux->y-1][aux->x].tipo){ // checa o espaço de cima
                case Empty: // se for vazio, prossegue para aquela direcao
                    area[aux->y-1][aux->x].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->y--;
                    break;
                case Player: // se for player, mata o player
                    return 1;
                    break;
                default: // se for qualquer outra coisa, troca de direcao
                    aux->dir = LEFT;
                    break;
            }
        if(aux->dir == LEFT)
            switch(area[aux->y][aux->x-1].tipo){ //checa o espaço da esquerda
                case Empty:
                   
                    area[aux->y][aux->x-1].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->x--;
                    break;
                case Player:
                    return 1;
                    break;
                default:
                    aux->dir = DOWN;
                    break;
            }
        if(aux->dir == DOWN)
            switch(area[aux->y+1][aux->x].tipo){ // checa o espaço de baixo
                case Empty:
                    
                    area[aux->y+1][aux->x].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->y++;
                    break;
                case Player:
                    return 1;
                    break;
                default:
                    aux->dir = RIGHT;
                    break;
            }
        if(aux->dir == RIGHT)
            switch(area[aux->y][aux->x+1].tipo){ // checa o espaço da direita
                case Empty:
                    area[aux->y][aux->x+1].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->x++;
                    break;
                case Player:
                    return 1;
                    break;
                default:
                    aux->dir = UP;
                    break;
            }
        temp = aux;
        if(area[aux->y-1][aux->x].tipo == Rock){
            explode(area, sprites, aux->x, aux->y, 0);
            deleta_nodo(bichos, temp);
        }
        aux = aux->next;
    }
    return 0;
}

int atualiza_butterflies(nodo **bichos, nodo **diamantes, tile **area, t_sprites sprites, jogador *player){
    nodo *aux = *bichos, *temp;
    short j;

    while(aux->next != NULL){     
        if(aux->dir == UP)
            switch(area[aux->y-1][aux->x].tipo){ // checa o espaço de cima
                case Empty: // se for vazio, prossegue para aquela direcao
                    area[aux->y-1][aux->x].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->y--;
                    break;
                case Player: // se for player, mata o player
                    return 1;
                    break;
                default: // se for qualquer outra coisa, troca de direcao
                    aux->dir = RIGHT;
                    break;
            }
        if(aux->dir == RIGHT)
            switch(area[aux->y][aux->x+1].tipo){ //checa o espaço da direita
                case Empty:
                    area[aux->y][aux->x+1].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->x++;
                    break;
                case Player:
                    return 1;
                    break;
                default:
                    aux->dir = DOWN;
                    break;
            }
        if(aux->dir == DOWN)
            switch(area[aux->y+1][aux->x].tipo){ // checa o espaço de baixo
                case Empty:
                    area[aux->y+1][aux->x].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->y++;
                    break;
                case Player:
                    return 1;
                    break;
                default:
                    aux->dir = LEFT;
                    break;
            }
        if(aux->dir == LEFT)
            switch(area[aux->y][aux->x-1].tipo){ // checa o espaço da esquerda
                case Empty:
                    area[aux->y][aux->x-1].tipo = Enemy;
                    area[aux->y][aux->x].tipo = Empty;
                    aux->x--;
                    break;
                case Player:
                    return 1;
                    break;
                default:
                    aux->dir = UP;
                    break;
            } 
        temp = aux;
        if(area[aux->y-1][aux->x].tipo == Rock){
            explode(area, sprites, aux->x, aux->y, 0);
            deleta_nodo(bichos, temp);
            for(j = -2; j < 4; j++)
                insere_nodo(diamantes, cria_nodo(0, aux->x+j, aux->y-1));
        }
        aux = aux->next;
    }
    return 0;
}

void destroi_lista(nodo **ini){ // destroi uma lista encadeada
    nodo *aux = *ini, *temp = *ini;
    
    while(aux->next != NULL){
        aux = aux->next;
        free(temp);
        temp = aux;
    }
    free(temp);
    *ini = NULL;
}