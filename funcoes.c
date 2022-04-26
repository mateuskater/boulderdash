#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "game.h"
#include "lista.h"
#include "init_sprites.h"
#include "display.h"
#include "som.h"

tile **aloca_area(int lin, int col){
    int i;
    tile **area;
    area = malloc(COL*sizeof(tile*));
    area[0] = calloc(LIN*COL, sizeof(tile));
    for(i=1; i<LIN; i++)
        area[i] = area[0] + i*COL;
    return area;
}

void inicializa_jogo(tile **area, jogador *player, nodo **pedras, nodo **diamantes, nodo **butterflies, nodo **fireflies, jogo *jogo){
    int i, j;
    char aux;
    char buf[128];

    snprintf(buf, sizeof(buf), "./resources/level%d.txt", jogo->n_level);
    ALLEGRO_FILE *level = al_fopen(buf, "r");
    nodo *novo_item;

    for(i = 0; i < LIN; i++)
        for(j = 0; j < COL; j++){
            al_fread(level, &aux, 1);
            if (aux != '\n'){
                switch(aux){
                    case 'W':
                        area[i][j].tipo = Wall;
                        break;
                    case 'w':
                        area[i][j].tipo = Brick;
                        break;
                    case 'd':
                        area[i][j].tipo = Diamond;
                        novo_item = cria_nodo(0, j, i);
                        *diamantes = insere_nodo(diamantes, novo_item);
                        break;
                    case 'r':
                        area[i][j].tipo = Rock;
                        novo_item = cria_nodo(0, j, i);
                        *pedras = insere_nodo(pedras, novo_item);
                        break;
                    case '.':
                        area[i][j].tipo = Dirt;
                        break;
                    case 'P':
                        area[i][j].tipo = SaidaFechada;
                        jogo->saida_x = j;
                        jogo->saida_y = i;
                        break;
                    case 'X':
                        area[i][j].tipo = Empty;
                        player->x = j;
                        player->y = i;
                        break;
                    case ' ':
                        area[i][j].tipo = Empty;
                        break;
                    case 'B':
                        area[i][j].tipo = Enemy;
                        novo_item = cria_nodo(0, j, i);
                        novo_item->dir = UP;
                        *butterflies = insere_nodo(butterflies, novo_item);
                        break;
                    case 'q':
                        area[i][j].tipo = Enemy;
                        novo_item = cria_nodo(0, j, i);
                        novo_item->dir = UP;
                        *fireflies = insere_nodo(fireflies, novo_item);
                        break;
                    default:
                        break;
                }
            }else j--;
        }
    al_fclose(level);
}

int colisao(tile **area, int direcao, jogador player){
    int px = player.x, py = player.y; //player.x e player.y, para simplificar
    int cima = area[py-1][px].tipo, //ladrilhos adjacentes
        esquerda = area[py][px-1].tipo,
        baixo = area[py+1][px].tipo,
        direita = area[py][px+1].tipo;
    
    if (direcao == UP && (cima == Empty || cima == Dirt || cima == Diamond)) // condicoes para nao ter colisao
        return 0; // retorna 0 se nao existir colisao
    if (direcao == LEFT && (esquerda == Empty || esquerda == Dirt || esquerda == Diamond))
        return 0;
    if (direcao == DOWN && (baixo == Empty || baixo == Dirt || baixo == Diamond))
        return 0;
    if (direcao == RIGHT && (direita == Empty || direita == Dirt || direita == Diamond))
        return 0;
    if (direcao == LEFT && (esquerda == Rock && area[py][px-2].tipo == Empty))
        return 0; // testa tambem os casos de poder empurrar
    if (direcao == RIGHT && (direita == Rock && area[py][px+2].tipo == Empty))
        return 0;
    return 1; // retorna 1 se existir colisao
}

void move_player(tile **area, jogador *player, int dir){
    switch(dir){
        case UP:
            area[player->y-1][player->x].tipo = Player;
            area[player->y][player->x].tipo = Empty;
            player->y--;
            break;
        case DOWN:
            area[player->y+1][player->x].tipo = Player;
            area[player->y][player->x].tipo = Empty;
            player->y++;
            break;
        case LEFT:
            area[player->y][player->x-1].tipo = Player;
            area[player->y][player->x].tipo = Empty;
            player->x--;
            player->dir = LEFT;
            break;
        case RIGHT:
            area[player->y][player->x+1].tipo= Player;
            area[player->y][player->x].tipo = Empty;
            player->x++;
            player->dir = RIGHT;
            break;
    }
}

int empurra(tile **area, jogador *player, nodo *pedras, int dir){ //direção será -1 para esquerda ou 1 para direita
    nodo *pedra_proxima;
    if (area[player->y][player->x+(dir)].tipo == Rock && area[player->y][player->x+(2*dir)].tipo == Empty){
        pedra_proxima = busca_nodo(pedras, player->x+dir, player->y);
        area[player->y][player->x+(2*dir)].tipo = Rock; // x+2 ou x-2 será pedra, dependendo da direção
        area[player->y][player->x+dir].tipo = Player; // x+1 ou x-1 será Player, dependendo da direção
        area[player->y][player->x].tipo = Empty; //substitui o espaço atual por vazio
        player->x+=dir;
        pedra_proxima->x+=dir;
        return 1; // retorna 1 se conseguiu empurrar
    }   
    return 0; // retorna 0 para sinalizar que não empurrou
}

void coleta_diamante(tile **area, jogador *player, jogo *jogo, t_sons sons, nodo **diamantes){
    nodo *diamante_coletado;
    diamante_coletado = busca_nodo(*diamantes, player->x, player->y);
    if(diamante_coletado != NULL){
        deleta_nodo(diamantes, diamante_coletado);
        player->score += 10;
        jogo->d_restantes--;
        al_play_sample(sons.diamante, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

void desenha_mapa(tile **area, t_sprites sprites){
    int i, j;

    for(i = 0; i < LIN; i++)
        for(j = 0; j < COL; j++){
            switch(area[i][j].tipo){
                case Wall:
                    al_draw_bitmap(sprites.wall, j*16, i*16 + OFF, 0);
                    break;
                case Dirt:
                    al_draw_bitmap(sprites.dirt, j*16, i*16 + OFF, 0);
                    break;
                case SaidaAberta:
                    al_draw_bitmap(sprites.exit, j*16, i*16 + OFF, 0);
                    break;
                case Brick:
                    al_draw_bitmap(sprites.brick, j*16, i*16 + OFF, 0);
                    break;
                default:
                    break;
            }
        }
}

void testa_init(bool test, const char *objeto){
    if(test) return;
    printf("%s não pôde ser inicializado\n", objeto);
    exit(1);
}

int morte(jogador player, t_sprites sprites, int frame){
    int i,j;
    for (i = -1; i <= 1; i++)
        for(j = -1; j <= 1; j++)
            al_draw_bitmap(sprites.morte[frame], player.x+j,player.y+i + OFF, 0);
    return 0;
}

void abre_saida(tile **area, jogo jogo){
    area[jogo.saida_y][jogo.saida_x].tipo = SaidaAberta;
}
