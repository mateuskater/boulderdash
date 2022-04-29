#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include "game.h"
#include "lista.h"
#include "init_sprites.h"
#include "display.h"
#include "som.h"

int main(){
    int frame_diamante = 0, frame_player = 0, frame_inimigos = 0, frame_explosao = 0, counter = 1, i = 1;
    jogador player;
    unsigned char key[ALLEGRO_KEY_MAX];
    tile **area;
    nodo *pedras = NULL;
    nodo *diamantes = NULL;
    nodo *butterflies = NULL;
    nodo *fireflies = NULL;
    nodo *diamante_coletado = NULL;
    jogo jogo = {0};
    ALLEGRO_SAMPLE_ID id_musica;
    int n_tela = 0; // para a tela de ajuda
    int passou = 0; // variavel para controlar se o personagem passou de fase

    testa_init(al_init(), "allegro"); // inicializa a engine do allegro
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    testa_init(queue, "queue");
    ALLEGRO_EVENT_QUEUE* qmenu = al_create_event_queue();
    testa_init(qmenu, "queue do menu");
    ALLEGRO_DISPLAY* disp = inicializa_tela(DEFAULT_WIDTH*2, DEFAULT_HEIGHT*2);
    testa_init(disp, "display");
    ALLEGRO_FONT* font = al_create_builtin_font();
    testa_init(font, "addon de fonte");
    memset(key, 0, sizeof(key));
    
    testa_init(al_install_keyboard(), "teclado");
    testa_init(al_install_mouse(), "mouse");

    testa_init(al_install_audio(), "audio");
    testa_init(al_init_acodec_addon(), "codecs de audio");
    testa_init(al_reserve_samples(16), "reserve samples");
    t_sons sons = carrega_sons();
    testa_init(al_init_image_addon(), "addon de imagem");
    t_sprites sprites = carrega_sprites();

    setup_transform(disp);

    ALLEGRO_BITMAP *logo = al_load_bitmap("./resources/bdlogo.png");
    testa_init(logo, "logo");

    ALLEGRO_BITMAP **help = calloc(3, sizeof(ALLEGRO_BITMAP*));
    gera_tela_help(help);

    ALLEGRO_BITMAP *heart = al_load_bitmap("./resources/heart.png");
    testa_init(heart, "heart");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(qmenu, al_get_keyboard_event_source());
    al_register_event_source(qmenu, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_mouse_event_source());

    int tela_jogo = 1; // para sinalizar se deve desenhar tela do jogo
    int tela_help = 0; // para sinalizar se deve desenhar a tela de help
    int flagBreak = 0; // uma flag para sair do while se estiver dentro de um switch
    ALLEGRO_EVENT event;

    jogo.reset = 0;
    jogo.n_level = 1;
    player.score = 0;
    player.vivo = 1;


    ALLEGRO_TIMER* timer_fps = al_create_timer(1.0 / 60.0);
    ALLEGRO_TIMER* timer_player = al_create_timer(1.0 / 10.0);
    ALLEGRO_TIMER* timer_anim = al_create_timer(1.0 / 10.0);
    ALLEGRO_TIMER* timer_pedras = al_create_timer(1.0 / 5.0);
    ALLEGRO_TIMER* relogio = al_create_timer(1.0);

    al_register_event_source(queue, al_get_timer_event_source(timer_fps));
    al_register_event_source(queue, al_get_timer_event_source(timer_player)); //registra as sources de evento
    al_register_event_source(queue, al_get_timer_event_source(timer_anim));
    al_register_event_source(queue, al_get_timer_event_source(timer_pedras));

    while(1){ // loop do menu
        al_draw_bitmap(logo, DEFAULT_WIDTH/5, 20, 0);
        // al_draw_scaled_bitmap(logo, 0, 0, larguraLogo, alturaLogo, 300, 50 + OFF, larguraLogo/3, alturaLogo/3, 0);
        al_draw_text(font, branco, DEFAULT_WIDTH/2, 250, ALLEGRO_ALIGN_CENTER, "Pressione a barra de espaço para jogar");
        al_flip_display();

        al_wait_for_event(qmenu, &event);
        
        if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
            switch(event.keyboard.keycode){
            case ALLEGRO_KEY_ESCAPE:
                exit(0);
                break;
            case ALLEGRO_KEY_BACKSPACE:
                exit(0);
                break;
            case ALLEGRO_KEY_ENTER:
                al_draw_text(font, aqua, 80, 200, 0 + OFF, "aperta espaço pora");
                break;
            case ALLEGRO_KEY_SPACE:
                flagBreak = 1;
                break;
            default :
                break;
            }
        }else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            exit(0);
        if (flagBreak){
            al_destroy_event_queue(qmenu);
            break;
        }
    }

    al_start_timer(timer_fps);
    al_start_timer(timer_player); //inicia os timers
    al_start_timer(timer_anim);
    al_start_timer(timer_pedras);

    player.vidas = 3;

    while(1){ // game loop
        
        area = aloca_area(DEFAULT_WIDTH,DEFAULT_HEIGHT);
        pedras = inicializa_lista(); //
        diamantes = inicializa_lista(); // inicializa listas
        butterflies = inicializa_lista(); //
        fireflies = inicializa_lista(); //
        inicializa_jogo(area, &player, &pedras, &diamantes, &butterflies, &fireflies, &jogo);
        jogo.n_butterflies = conta_nodos(butterflies);
        jogo.n_diams = conta_nodos(diamantes) + 6*jogo.n_butterflies;
        jogo.d_restantes = (int)jogo.n_diams*(0.7);
        al_start_timer(relogio);
        al_play_sample(sons.musica, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_musica);

        while(player.vivo && !passou && !jogo.reset){ // game loop

            al_wait_for_event(queue, &event);

            switch(event.type){
                case ALLEGRO_EVENT_TIMER:
                    if(event.timer.source == timer_anim){
                        frame_player = counter % 7;
                        frame_diamante = counter % 8;
                        frame_inimigos = counter % 4;
                        frame_explosao = counter % 5;
                        counter++;
                    }

                    if(event.timer.source == timer_pedras){
                        if(atualiza_objetos(&pedras, area, sprites, 'r')) // aqui é usado um método binário de verificação.
                            player.vivo = explode(area, sprites, player.x, player.y, 0); // se alguma dessas funções retorna 1, o player morreu
                        if(atualiza_objetos(&diamantes, area, sprites, 'd'))
                            player.vivo = explode(area, sprites, player.x, player.y, 0);
                        if(atualiza_fireflies(&fireflies, area, sprites, &player))
                            player.vivo = explode(area, sprites, player.x, player.y, 0);
                        if(atualiza_butterflies(&butterflies, &diamantes, area, sprites, &player))
                            player.vivo = explode(area, sprites, player.x, player.y, 0);
                    }

                    if(event.timer.source == timer_player){
                        if((key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W]) && !colisao(area, UP, player)){
                            if(area[player.y-1][player.x].tipo == SaidaAberta)
                                passou = 1;
                            move_player(area, &player, UP);
                            coleta_diamante(area, &player, &jogo, sons, &diamantes);
                        }

                        else if((key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S]) && !colisao(area, DOWN, player)){
                            if(area[player.y+1][player.x].tipo == SaidaAberta)
                                passou = 1;
                            move_player(area, &player, DOWN);        
                            coleta_diamante(area, &player, &jogo, sons, &diamantes);
                        }

                        else if((key[ALLEGRO_KEY_LEFT] || key[ALLEGRO_KEY_A]) && !colisao(area, LEFT, player)){
                            if(area[player.y][player.x-1].tipo == SaidaAberta)
                                passou = 1;
                            else if (area[player.y][player.x-1].tipo == Rock)
                                empurra(area, &player, pedras, LEFT);
                            else
                                move_player(area, &player, LEFT);
                            coleta_diamante(area, &player, &jogo, sons, &diamantes); 
                        }

                        else if((key[ALLEGRO_KEY_RIGHT] || key[ALLEGRO_KEY_D]) && !colisao(area, RIGHT, player)){
                            if(area[player.y][player.x+1].tipo == SaidaAberta)
                                passou = 1;
                            else if (area[player.y][player.x+1].tipo == Rock)
                                empurra(area, &player, pedras, RIGHT);
                            else
                                move_player(area, &player, RIGHT);
                            coleta_diamante(area, &player, &jogo, sons, &diamantes);
                        }else player.dir = STILL;

                        if(area[player.y][player.x].tipo == Dirt){ //cava
                            area[player.y][player.x].tipo = Empty;
                            al_play_sample(sons.terra, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        }
                            
                        if(area[player.y][player.x].tipo == Diamond){ // coleta diamante
                            area[player.y][player.x].tipo = Empty;
                            diamante_coletado = busca_nodo(diamantes, player.x, player.y);
                            deleta_nodo(&diamantes, diamante_coletado);
                            jogo.d_restantes--;
                            player.score+=10;
                        }
                    }

                    jogo.t_restante = TEMPO_LIMITE - al_get_timer_count(relogio);

                    if(jogo.d_restantes <= 0)
                        abre_saida(area, jogo);
                    if (jogo.t_restante == 0){
                        player.vivo = 0;
                        al_rest(2.0);
                    }
                    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                        key[i] &= KEY_SEEN;
                    tela_jogo = 1;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                    if (key[ALLEGRO_KEY_PGDN] && jogo.n_level > 1){
                        jogo.n_level--;
                        jogo.reset = 1;
                    }else
                    if (key[ALLEGRO_KEY_PGUP] && jogo.n_level < 10){
                        jogo.n_level++;
                        jogo.reset = 1;
                    }else
                    if (key[ALLEGRO_KEY_H]){
                        tela_help = 1;
                        tela_jogo = 0;
                    }else
                    if(key[ALLEGRO_KEY_M]){
                        sprites.rock = sprites.prof;
                    }else
                    if(key[ALLEGRO_KEY_R]){
                        player.vivo = 0;
                    }
                    if(key[ALLEGRO_KEY_ESCAPE])
                        exit(0);
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if ((event.mouse.button & 2) && jogo.n_level > 1){
                        jogo.n_level--;
                        jogo.reset = 1;
                    }else
                    if((event.mouse.button & 1) && jogo.n_level < 10){
                        jogo.n_level++;
                        jogo.reset = 1;
                    }
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    key[event.keyboard.keycode] &= KEY_RELEASED;
                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    exit(0);
            }

            while(tela_help){
                al_stop_timer(relogio);
                al_stop_timer(timer_anim);
                al_stop_timer(timer_player);
                al_stop_timer(timer_pedras);
                al_set_target_backbuffer(disp);
                al_draw_bitmap(help[n_tela], 0, 0, 0);
                al_flip_display();
                al_wait_for_event(queue, &event);
                if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                    if(key[ALLEGRO_KEY_H]){
                        tela_help = 0;
                        tela_jogo = 1;
                        al_resume_timer(relogio);
                        al_resume_timer(timer_anim);
                        al_resume_timer(timer_player);
                        al_resume_timer(timer_pedras);
                    }else if(key[ALLEGRO_KEY_RIGHT] && n_tela < 2)
                        n_tela++;
                    else if(key[ALLEGRO_KEY_LEFT] && n_tela > 0)
                        n_tela--;
                }else if(event.type == ALLEGRO_EVENT_KEY_UP)
                    key[event.keyboard.keycode] &= KEY_RELEASED;
            }

            if(tela_jogo && event.timer.source == timer_fps){
                if(jogo.d_restantes == 0)
                    al_draw_text(font, branco, 300, 5, 0, "Saída Aberta!");
                al_clear_to_color(preto);
                al_draw_textf(font, branco, 10, 5, 0, "Diamantes restantes: %d", jogo.d_restantes);
                al_draw_textf(font, branco, 200, 5, 0, "Score: %d", player.score);
                al_draw_textf(font, branco, 300, 5, 0, "Vidas:");
                for(int j = 1; j <= player.vidas; j++) // desenha as vidas
                    al_draw_bitmap(heart, 340+(10*j), 4, 0);
                // al_draw_textf(font, branco, 300, 5, 0, "%d Pedras %d Diamantes", conta_nodos(pedras), conta_nodos(diamantes));
                al_draw_textf(font, branco, 500, 5, 0, "Time: %d", jogo.t_restante);
                desenha_mapa(area, sprites);
                if (player.dir == STILL){ // desenha o player
                    al_draw_bitmap(sprites.player[frame_player], player.x*16, player.y*16 + OFF, 0);
                } else if (player.dir == LEFT){
                    al_draw_bitmap(sprites.player_left[frame_player], player.x*16, player.y*16 + OFF, 0);
                }else if (player.dir == RIGHT){
                    al_draw_bitmap(sprites.player_right[frame_player], player.x*16, player.y*16 + OFF, 0);
                }
                desenha_pedras(pedras, sprites);
                desenha_diamantes(diamantes, sprites, frame_diamante);
                desenha_butterflies(butterflies, sprites, frame_inimigos);
                desenha_fireflies(fireflies, sprites, frame_inimigos);
                al_flip_display();
                tela_jogo = 0;
            }
        }

        al_stop_timer(relogio);
        al_set_timer_count(relogio, 0); // reseta o relogio do jogo
        al_stop_sample(&id_musica);
        if(passou){ // se passou, sobe o nivel 
            jogo.n_level++;
            al_play_sample(sons.vitoria, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }

        if(player.vivo == 0){ // se morreu, 
            al_rest(2.0);
            for(i = 0; i < 5; i++){
                al_wait_for_event(queue, &event);
                if(event.type == ALLEGRO_EVENT_TIMER && event.timer.source == timer_anim)
                    explode(area, sprites, player.x, player.y, frame_explosao);
                frame_explosao = counter % 5;
            }
            player.vidas--;
            al_play_sample(sons.morreu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }

        free(area[0]);
        free(area);
        destroi_lista(&pedras);
        destroi_lista(&diamantes);
        destroi_lista(&butterflies);
        destroi_lista(&fireflies);
        player.vivo = 1;
        jogo.reset = 0;
        passou = 0;
    }

    free(area[0]);
    free(area);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer_fps);
    al_destroy_timer(timer_player);
    al_destroy_timer(timer_anim);
    al_destroy_timer(relogio);
    al_destroy_event_queue(queue);
    al_destroy_event_queue(qmenu);
    destroi_audio(&sons);
    destroi_lista(&pedras);
    destroi_lista(&diamantes);
    destroi_lista(&butterflies);
    destroi_lista(&fireflies);

    return 0;
}