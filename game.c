#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include "bdash.h"
#include "init_sprites.h"
#include "lista_pedras.h"

tile **aloca_area(int lin, int col){
    int i;
    tile **area;
    area = malloc(COL*sizeof(tile*));
    area[0] = calloc(LIN*COL, sizeof(tile));
    for(i=1; i<LIN; i++)
        area[i] = area[0] + i*COL;
    return area;
}

void inicializa_jogo(tile **area, jogador *player, nodo **pedras){
    int i, j;
    char aux;
    ALLEGRO_FILE *level = al_fopen("./resources/level2.txt", "r");
    nodo *nova_pedra;

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
                        break;
                    case 'r':
                        area[i][j].tipo = Rock;
                        nova_pedra = cria_nodo(0, 0, j, i);
                        *pedras = insere_nodo(pedras, nova_pedra);
                        break;
                    case '.':
                        area[i][j].tipo = Dirt;
                        break;
                    case 'P':
                        area[i][j].tipo = Exit;
                        break;
                    case 'X':
                        area[i][j].tipo = Empty;
                        player->x = j;
                        player->y = i;
                        break;
                    case ' ':
                        area[i][j].tipo = Empty;
                        break;
                    case '\n':
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
    
    if (direcao == UP && (cima == Brick || cima == Wall || cima == Rock))
        return 1;
    if (direcao == LEFT && (esquerda == Brick || esquerda == Wall || esquerda == Rock))
        return 1;
    if (direcao == DOWN && (baixo == Brick || baixo == Wall || baixo == Rock))
        return 1;
    if (direcao == RIGHT && (direita == Brick || direita == Wall || direita == Rock))
        return 1;
    
    return 0;
}

void empurra(tile **area, int dir){


}

void reseta_level(){

}

void desenha_mapa(tile **area, t_sprites sprites){
    int i, j;

    for(i = 0; i < LIN; i++)
        for(j = 0; j < COL; j++){
            switch(area[i][j].tipo){
                case Empty:
                    break;
                case Wall:
                    al_draw_bitmap(sprites.wall, j*16, i*16, 0);
                    break;
                case Dirt:
                    al_draw_bitmap(sprites.dirt, j*16, i*16, 0);
                    break;
                case Player:
                    break;
                case Rock:
                    break;
                case Exit:
                    al_draw_bitmap(sprites.rock, j*16, i*16, 0);
                    break;
                case Brick:
                    al_draw_bitmap(sprites.brick, j*16, i*16, 0);
                    break;
                case Diamond:
                    al_draw_bitmap(sprites.diamond[0], j*16, i*16, 0);
                    break;
            }
        }
}

void testa_init(bool test, const char *objeto){
    if(test) return;
    printf("%s não pôde ser inicializado\n", objeto);
    exit(1);
}

void morte(jogador player, t_sprites sprites, ALLEGRO_TIMER *t){
    int i,j,f = 0;
    // al_stop_timer(t);
    for (f = 0; f < 5; f++)
        for (i = -1; i <= 1; i++)
            for(j = -1; j <= 1; j++){
                al_draw_bitmap(sprites.morte[f], player.x+j,player.y+i, 0);
            }
}

int main(int argc, char *argv[]){
    int wid, hei, prev_x, prev_y, m = 0, f; //largura, altura, posiçao anterior do player
    jogador player;
    unsigned char key[ALLEGRO_KEY_MAX];
    tile **area;
    nodo *pedras = inicializa_lista();

    memset(key, 0, sizeof(key));
    
    if (argv[1] != NULL || argv[2] != NULL){ // valores padrão caso tamanho não for informado
        wid = DEFAULT_WIDTH;
        hei = DEFAULT_HEIGHT;
    }else{
        wid = atoi(argv[1]);
        hei = atoi(argv[2]);
    }

    area = aloca_area(wid,hei);
    
    testa_init(al_init(), "allegro");
    testa_init(al_install_keyboard(), "teclado");
    testa_init(al_init_image_addon(), "addon de imagem");
    testa_init(al_init_primitives_addon(), "primitivos");

    ALLEGRO_TIMER* timer_fps = al_create_timer(1.0 / 60.0);
    testa_init(timer_fps, "timer fps");

    ALLEGRO_TIMER* timer_player = al_create_timer(1.0 / 8.0);
    testa_init(timer_player, "timer 2");

    ALLEGRO_TIMER* timer_game = al_create_timer(1.0 / 15.0);
    testa_init(timer_game, "timer do jogo");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    testa_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = inicializa_tela(DEFAULT_WIDTH*2, DEFAULT_HEIGHT*2);
    testa_init(disp, "display");

    ALLEGRO_TRANSFORM t = setup_transform(disp);

    ALLEGRO_FONT* font = al_create_builtin_font();
    testa_init(font, "addon de fonte");

    t_sprites sprites = carrega_sprites();

    ALLEGRO_KEYBOARD_STATE ks;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer_fps));
    al_register_event_source(queue, al_get_timer_event_source(timer_player));
    al_register_event_source(queue, al_get_timer_event_source(timer_game));

    bool feito = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer_fps);
    al_start_timer(timer_player);
    al_start_timer(timer_game);
    
    inicializa_jogo(area, &player, &pedras);
    atualiza_pedras(&pedras, area, sprites);



    while(1){ // game loop
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&ks);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == timer_game){
                    m = f % 7;
                }
                if(event.timer.source == timer_player){
                    prev_x = player.x;
                    prev_y = player.y;
                    if((key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W]) && !colisao(area, UP, player)){
                        player.y--;
                    }
                    else if((key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S]) && !colisao(area, DOWN, player)){
                        player.y++;
                    }
                    else if((key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A]) && !colisao(area, LEFT, player)){
                        player.x--;
                        player.dir = LEFT;
                    }
                    else if((key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D]) && !colisao(area, RIGHT, player)){
                        player.x++;
                        player.dir = RIGHT;
                    }
                    
                    // if((key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A]) && area[player.y][player.x-1].tipo == Rock && area[player.y][player.x-2].tipo == Empty){
                        // player.x--;
                        // player.dir = LEFT;
                        // area[player.x-2][player.y].tipo = Rock;
                    // }
                    player.dir = STILL;
                    if(prev_x != player.x || prev_y != player.y){ // se a posicao atual for diferente da anterior, atualiza os conteudos
                        area[player.y][player.x].tipo = Player;
                        area[prev_y][prev_x].tipo = Empty;
                    }
                    atualiza_pedras(&pedras, area, sprites);
                    if(area[player.y-1][player.x].tipo == Rock){
                        morte(player, sprites, timer_player);
                    }
                    if(area[player.y][player.x].tipo == Dirt) //cava
                        area[player.y][player.x].tipo = Empty;
                }
                if(area[player.y][player.x].tipo == Diamond){ // coleta diamante
                    area[player.y][player.x].tipo = Empty;
                    player.score+=10;
                }

                if(key[ALLEGRO_KEY_ESCAPE])
                    exit(0);
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                feito = true;
                exit(0);
        }

        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), wid/2, hei/2, 0, "Hello world!");
            desenha_mapa(area, sprites);
            if (player.dir == STILL){
                al_draw_bitmap(sprites.player[m], player.x*16, player.y*16, 0);
                f++;
            } else if (player.dir == LEFT){
                al_draw_bitmap(sprites.player_left[m], player.x*16, player.y*16, 0);
                f++;
            }else if (player.dir == RIGHT){
                al_draw_bitmap(sprites.player_right[m], player.x*16, player.y*16, 0);
                f++;
            }
            desenha_pedras(pedras, sprites);
            al_flip_display();

            redraw = false;
        }
    }
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer_fps);
    al_destroy_timer(timer_player);
    al_destroy_timer(timer_game);
    al_destroy_event_queue(queue);
    free(area);
    destroi_lista(pedras);

    return 0;
}