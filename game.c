#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "lista.h"
#include "init_sprites.h"
#include "display.h"

tile **aloca_area(int lin, int col){
    int i;
    tile **area;
    area = malloc(COL*sizeof(tile*));
    area[0] = calloc(LIN*COL, sizeof(tile));
    for(i=1; i<LIN; i++)
        area[i] = area[0] + i*COL;
    return area;
}

void inicializa_jogo(tile **area, jogador *player, nodo **pedras, nodo **diamantes, jogo *jogo){
    int i, j;
    char aux;
    char buf[128];

    snprintf(buf, sizeof(buf), "./resources/level%d.txt", jogo->n_level);
    ALLEGRO_FILE *level = al_fopen(buf, "r");
    nodo *nova_pedra, *novo_diam;

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
                        novo_diam = cria_nodo(0, j, i);
                        *diamantes = insere_nodo(diamantes, novo_diam);
                        break;
                    case 'r':
                        area[i][j].tipo = Rock;
                        nova_pedra = cria_nodo(0, j, i);
                        *pedras = insere_nodo(pedras, nova_pedra);
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

void coleta_diamante(tile **area, jogador *player, jogo *jogo, nodo **diamantes){
    nodo *diamante_coletado;
    diamante_coletado = busca_nodo(*diamantes, player->x, player->y);
    if(diamante_coletado != NULL){
        deleta_nodo(diamantes, diamante_coletado);
        player->score += 10;
        jogo->d_restantes--;
    }
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
                    al_draw_bitmap(sprites.wall, j*16, i*16 + OFF, 0);
                    break;
                case Dirt:
                    al_draw_bitmap(sprites.dirt, j*16, i*16 + OFF, 0);
                    break;
                case Player:
                    break;
                case Rock:
                    break;
                case SaidaFechada:
                    break;
                case SaidaAberta:
                    al_draw_bitmap(sprites.exit, j*16, i*16 + OFF, 0);
                    break;
                case Brick:
                    al_draw_bitmap(sprites.brick, j*16, i*16 + OFF, 0);
                    break;
                case Diamond:
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
        for(j = -1; j <= 1; j++){
            al_draw_bitmap(sprites.morte[frame], player.x+j,player.y+i + OFF, 0);
        }
    return 1;
}

void abre_saida(tile **area, jogo jogo){
    area[jogo.saida_y][jogo.saida_x].tipo = SaidaAberta;
}

int main(int argc, char *argv[]){
    int frame_diamante = 0, frame_player = 0, counter = 1, i = 1; //largura, altura, posiçao anterior do player
    jogador player;
    unsigned char key[ALLEGRO_KEY_MAX];
    tile **area;// = aloca_area(DEFAULT_WIDTH,DEFAULT_HEIGHT);
    nodo *pedras = inicializa_lista(); // = inicializa_lista();
    nodo *diamantes = inicializa_lista(); // = inicializa_lista();
    nodo *pedra_acima = NULL;
    nodo *diamante_coletado = NULL;
    int morto = 0;
    int iter = 0;
    jogo jogo = {0, 0, 0};
    int frame_explosao = 0;
    int passou = 0;

    memset(key, 0, sizeof(key));
    
    player.score = 0;
    
    testa_init(al_init(), "allegro");
    testa_init(al_install_keyboard(), "teclado");
    testa_init(al_init_image_addon(), "addon de imagem");

    ALLEGRO_TIMER* timer_fps = al_create_timer(1.0 / 60.0);
    ALLEGRO_TIMER* timer_player = al_create_timer(1.0 / 10.0);
    ALLEGRO_TIMER* timer_anim = al_create_timer(1.0 / 7.0);
    ALLEGRO_TIMER* timer_pedras = al_create_timer(1.0 / 5.0);
    ALLEGRO_TIMER* relogio = al_create_timer(1.0);

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    testa_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = inicializa_tela(DEFAULT_WIDTH*2, DEFAULT_HEIGHT*2);
    testa_init(disp, "display");

    setup_transform(disp);

    ALLEGRO_FONT* font = al_create_builtin_font();
    testa_init(font, "addon de fonte");

    t_sprites sprites = carrega_sprites();

    ALLEGRO_BITMAP *gato = al_load_bitmap("./resources/gato.png");
    testa_init(gato, "gatin");

    ALLEGRO_KEYBOARD_STATE ks;

    ALLEGRO_EVENT_QUEUE* qmenu = al_create_event_queue();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));

    al_register_event_source(qmenu, al_get_keyboard_event_source());
    al_register_event_source(qmenu, al_get_display_event_source(disp));

    bool redraw = true;
    ALLEGRO_EVENT event;

    int alturaGato = al_get_bitmap_height(gato);
    int larguraGato = al_get_bitmap_width(gato);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    int flagBreak = 0;

    jogo.n_level = 1;

    while(1){ // menu

        al_draw_text(font, al_map_rgb(0, 255, 255), DEFAULT_WIDTH/2, 10, ALLEGRO_ALIGN_CENTRE, "boder dash");
        al_draw_scaled_bitmap(gato, 0, 0, larguraGato, alturaGato, 300, 50 + OFF, larguraGato/3, alturaGato/3, 0);
        al_draw_text(font, al_map_rgb(0, 255, 255), 50, 100 + OFF, 0, "Pressione tecla");
        al_draw_text(font, al_map_rgb(255,255,255), DEFAULT_WIDTH/2, DEFAULT_HEIGHT/2, ALLEGRO_ALIGN_CENTER, "Pressione a barra de espaço para jogar");
        al_flip_display();

        al_wait_for_event(qmenu, &event);
        al_get_keyboard_state(&ks);
        
        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            switch(event.keyboard.keycode){
            case ALLEGRO_KEY_ESCAPE:
                exit(0);
                break;
            case ALLEGRO_KEY_BACKSPACE:
                exit(0);
                break;
            case ALLEGRO_KEY_ENTER:
                al_draw_text(font, al_map_rgb(0, 255, 255), 80, 200, 0 + OFF, "aperta espaço pora");
                break;
            case ALLEGRO_KEY_SPACE:
                flagBreak = 1;
                break;
            default :
                /// Add character to our string
                break;
            }
        } 
        if (flagBreak){
            al_destroy_event_queue(qmenu);
            break;
        }
    }

    al_register_event_source(queue, al_get_timer_event_source(timer_fps));
    al_register_event_source(queue, al_get_timer_event_source(timer_player));
    al_register_event_source(queue, al_get_timer_event_source(timer_anim));
    al_register_event_source(queue, al_get_timer_event_source(timer_pedras));

    al_start_timer(timer_fps);
    al_start_timer(timer_player);
    al_start_timer(timer_anim);
    al_start_timer(timer_pedras);

    while(1){ // game loop
        
        area = aloca_area(DEFAULT_WIDTH,DEFAULT_HEIGHT);
        pedras = inicializa_lista();
        diamantes = inicializa_lista();
        inicializa_jogo(area, &player, &pedras, &diamantes, &jogo);
        jogo.n_diams = conta_nodos(diamantes);
        jogo.d_restantes = jogo.n_diams;
        al_start_timer(relogio);
        // atualiza_objetos(&pedras, area, sprites, 'r');
        // atualiza_objetos(&diamantes, area, sprites, 'd');

        while(!morto && !passou){ // game loop

            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&ks);

            switch(event.type){
                case ALLEGRO_EVENT_TIMER:
                    if(event.timer.source == timer_anim){
                        frame_player = counter % 7;
                        frame_diamante = counter % 8;
                        counter++;
                    }

                    if(event.timer.source == timer_fps){
                        jogo.t_restante = TEMPO_LIMITE - al_get_timer_count(relogio);
                        if (jogo.t_restante == 0)
                            morto = 1;
                    }
                    if(event.timer.source == timer_pedras){
                        atualiza_objetos(&pedras, area, sprites, 'r');
                        atualiza_objetos(&diamantes, area, sprites, 'd');
                        if(area[player.y-1][player.x].tipo == Rock){
                            pedra_acima = busca_nodo(pedras, player.x, player.y-1);
                            if(pedra_acima != NULL && pedra_acima->caindo == 1)
                                morto = morte(player, sprites, 0);
                        }
                    }

                    if(event.timer.source == timer_player){

                        if((key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W]) && !colisao(area, UP, player)){
                            if(area[player.y-1][player.x].tipo == SaidaAberta)
                                passou = 1;
                            move_player(area, &player, UP);
                            coleta_diamante(area, &player, &jogo, &diamantes); 
                        }
                        else if((key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S]) && !colisao(area, DOWN, player)){
                            if(area[player.y-1][player.x].tipo == SaidaAberta)
                                passou = 1;
                            move_player(area, &player, DOWN);        
                            coleta_diamante(area, &player, &jogo, &diamantes);            
                        }
                        else if((key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A]) && !colisao(area, LEFT, player)){
                            if(area[player.y-1][player.x].tipo == SaidaAberta)
                                passou = 1;
                            else if (area[player.y][player.x-1].tipo == Rock)
                                empurra(area, &player, pedras, ESQUERDA);
                            else
                                move_player(area, &player, LEFT);
                            coleta_diamante(area, &player, &jogo, &diamantes); 
                        }
                        else if((key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D]) && !colisao(area, RIGHT, player)){
                            if(area[player.y-1][player.x].tipo == SaidaAberta)
                                passou = 1;
                            else if (area[player.y][player.x+1].tipo == Rock)
                                empurra(area, &player, pedras, DIREITA);
                            else
                                move_player(area, &player, RIGHT);
                            coleta_diamante(area, &player, &jogo, &diamantes); 
                        }else player.dir = STILL;

                        if(area[player.y][player.x].tipo == Dirt) //cava
                            area[player.y][player.x].tipo = Empty;
                            
                        if(area[player.y][player.x].tipo == Diamond){ // coleta diamante
                            area[player.y][player.x].tipo = Empty;
                            diamante_coletado = busca_nodo(diamantes, player.x, player.y);
                            deleta_nodo(&diamantes, diamante_coletado);
                            jogo.d_restantes--;
                            player.score+=10;
                        }

                        if(jogo.d_restantes <= 0){
                            abre_saida(area, jogo);
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
                    exit(0);
            }

            if(redraw && al_is_event_queue_empty(queue) && event.timer.source == timer_fps){
                if(jogo.d_restantes == 0){
                    al_clear_to_color(al_map_rgb(255,255,255));
                    al_draw_text(font, al_map_rgb(255,255,255), 300, 5, 0, "Saída Aberta!");
                }
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_textf(font, al_map_rgb(255,255,255), 10, 5, 0, "Diamantes restantes: %d", jogo.d_restantes);
                al_draw_textf(font, al_map_rgb(255,255,255), 200, 5, 0, "Score: %d", player.score);
                // al_draw_textf(font, al_map_rgb(255,255,255), 300, 5, 0, "%d Pedras %d Diamantes", conta_nodos(pedras), conta_nodos(diamantes));
                al_draw_textf(font, al_map_rgb(255,255,255), 300, 5, 0, "");
                al_draw_textf(font, al_map_rgb(255,255,255), 500, 5, 0, "Time: %d", jogo.t_restante);
                desenha_mapa(area, sprites);
                if (player.dir == STILL){
                    al_draw_bitmap(sprites.player[frame_player], player.x*16, player.y*16 + OFF, 0);
                } else if (player.dir == LEFT){
                    al_draw_bitmap(sprites.player_left[frame_player], player.x*16, player.y*16 + OFF, 0);
                }else if (player.dir == RIGHT){
                    al_draw_bitmap(sprites.player_right[frame_player], player.x*16, player.y*16 + OFF, 0);
                }
                desenha_pedras(pedras, sprites);
                desenha_diamantes(diamantes, sprites, frame_diamante);
                al_flip_display();
                redraw = false;
            }
            iter++;
        }
        if(passou)
            jogo.n_level++;
        for(i = 0; i < 5; i++){
            al_wait_for_event(queue, &event);
            if(event.type == ALLEGRO_EVENT_TIMER && event.timer.source == timer_anim)
                morte(player, sprites, frame_explosao);
            frame_explosao = counter % 5;
        }
        free(area[0]);
        free(area);
        destroi_lista(&pedras);
        destroi_lista(&diamantes);
        al_stop_timer(relogio);
        morto = 0;
        passou = 0;
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer_fps);
    al_destroy_timer(timer_player);
    al_destroy_timer(timer_anim);
    al_destroy_event_queue(queue);
    free(area[0]);
    free(area);
    destroi_lista(&pedras);
    destroi_lista(&diamantes);

    return 0;
}