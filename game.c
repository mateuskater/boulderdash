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

void noop(void){ }

tile **aloca_area(int lin, int col){
    int i;
    tile **area;
    area = malloc(COL*sizeof(tile*));
    area[0] = calloc(LIN*COL, sizeof(tile));
    for(i=1; i<LIN; i++)
        area[i] = area[0] + i*COL;
    return area;
}

void inicializa_jogo(tile **area, jogador *player){
    int i, j;
    char aux;
    nodo *pedra = inicializa_lista();
    ALLEGRO_FILE *level = al_fopen("./resources/level1.txt", "r");

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
                        insere(pedra, STILL, 1, j, i);
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
void desenha_jogo(tile **area, ALLEGRO_BITMAP *sprites){
    int i, j;

    for(i = 0; i < LIN; i++)
        for(j = 0; j < COL; j++){
            switch(area[i][j].tipo){
                case Empty:
                    noop();
                    break;
                case Wall:
                    al_draw_bitmap(sprites->player[0], j*16, i*16+1, 0);
                    break;
                case Dirt:
                    al_draw_bitmap_region(sprites, 48, 48, 16, 16, j*16, i*16+1, 0);
                    break;
                case Player:
                    al_draw_bitmap_region(sprites, 0, 0, 16, 16, j*16, i*16+1, 0);
                    break;
                case Rock:
                    al_draw_bitmap_region(sprites, 80, 48, 16, 16, j*16, i*16+1, 0);
                    break;
                case Exit:
                    al_draw_bitmap_region(sprites, 80, 64, 16, 16, j*16, i*16+1, 0);
                    break;
                case Brick:
                    al_draw_bitmap_region(sprites, 32, 48, 16, 16, j*16, i*16+1, 0);
                    break;
                case Diamond:
                    al_draw_bitmap_region(sprites, 0, 64, 16, 16, j*16, i*16+1, 0);
                    break;
            }
        }
}

void testa_init(bool test, const char *objeto){
    if(test) return;
    printf("%s não pôde ser inicializado\n", objeto);
    exit(1);
}

int main(int argc, char *argv[]){
    int wid, hei, i;
    jogador player;
    unsigned char key[ALLEGRO_KEY_MAX];
    tile **area;

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

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    testa_init(timer, "timer");

    ALLEGRO_TIMER* timer_player = al_create_timer(1.0 / 8.0);
    testa_init(timer, "timer 2");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    testa_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = inicializa_tela(1440, 720);
    testa_init(disp, "display");

    ALLEGRO_TRANSFORM t = setup_transform(disp);

    ALLEGRO_FONT* font = al_create_builtin_font();
    testa_init(font, "addon de fonte");

    t_sprites* sprites = carrega_sprites;
    testa_init(sprites, "sprites");

    ALLEGRO_BITMAP* wall = al_load_bitmap("./resources/steel.png");
    testa_init(wall, "parede");

    // ALLEGRO_BITMAP* rockford = al_create_sub_bitmap(sprites, 0, 0, 16, 16);

    ALLEGRO_KEYBOARD_STATE ks;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(timer_player));

    bool feito = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    al_start_timer(timer_player);
    inicializa_jogo(area, &player);

    while(1){
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&ks);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == timer_player){
                    if((key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W]) && !colisao(area, UP, player))
                        player.y--;
                    if((key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S]) && !colisao(area, DOWN, player))
                        player.y++;
                    if((key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A]) && !colisao(area, LEFT, player))
                        player.x--;
                    if((key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D]) && !colisao(area, RIGHT, player))
                        player.x++;
                    if(area[player.y][player.x].tipo == Dirt){
                        area[player.y][player.x].tipo = Empty;
                        if(area[player.y-1][player.x].tipo == Rock){
                            area[player.y][player.x].tipo = Rock;
                            area[player.y-1][player.x].tipo = Empty;
                        }
                    }
                    if(area[player.y][player.x].tipo == Diamond){
                        area[player.y][player.x].tipo = Empty;
                        player.score+=10;
                    }    
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
            desenha_jogo(area, sprites);
            // al_draw_bitmap(rockford, player.x*16, player.y*16, 0);
            al_flip_display();

            redraw = false;
        }
    }
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    free(area);

    return 0;
}