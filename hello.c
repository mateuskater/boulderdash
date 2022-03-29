#include <stdio.h>
#include <stdlib.h>
#include "bdash.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

#define KEY_SEEN 1
#define KEY_RELEASED 2

typedef struct tile{
    //enum tipo{Empty = 0, Wall, Dirt, Player, Rock, Exit, Diamond}tipo;
    int tipo;
    int x, y;
}tile;

void inicializa_area(tile ***area, int w, int h){
    int i,j;
    area = malloc(23*sizeof(tile*));
    area[0] = calloc(23*40, sizeof(tile));
    for(i=1; i<23; i++)
        area[i] = area[0] + i*40;
    for(i=0;i<23;i++)
        for(j=0;j<23;j++){
            area[i][j]->x = i*w/40;
            area[i][j]->y = i*h/23;
        }
    area[0][0]->x = 0;
    area[0][0]->y = 0;
}

void desenha_jogo(void){

}

void testa_init(bool test, const char *objeto){
    if(test) return;
    printf("%s não pôde ser inicializado\n", objeto);
    exit(1);
}

int main(int argc, char *argv[]){
    int wid, hei;
    jogador mysha;
    unsigned char key[ALLEGRO_KEY_MAX];
    tile **area = NULL;

    memset(key, 0, sizeof(key));
    
    if (argv[1] == NULL || argv[2] == NULL){ // valores padrão caso tamanho não for informado
        wid = 640;
        hei = 480;
    }else{
        wid = atoi(argv[1]);
        hei = atoi(argv[2]);
    }
    
    inicializa_area(&area, wid, hei);

    mysha.x = wid/2;
    mysha.y = hei/2;
    
    testa_init(al_init(), "allegro");
    testa_init(al_install_keyboard(), "teclado");
    testa_init(al_init_image_addon(), "addon de imagem");
    testa_init(al_init_primitives_addon(), "primitivos");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    testa_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    testa_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(wid, hei);
    testa_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    testa_init(font, "addon de fonte");

    ALLEGRO_BITMAP* myshabmp = al_load_bitmap("./resources/mysha.png");
    testa_init(myshabmp, "ratinho");

    ALLEGRO_BITMAP* wall = al_load_bitmap("./resources/steel.png");
    testa_init(wall, "parede");

    ALLEGRO_KEYBOARD_STATE ks;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool feito = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);

    while(1){
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&ks);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_UP])
                    mysha.y--;
                if(key[ALLEGRO_KEY_DOWN])
                    mysha.y++;
                if(key[ALLEGRO_KEY_LEFT])
                    mysha.x--;
                if(key[ALLEGRO_KEY_RIGHT])
                    mysha.x++;

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
                break;
        }

        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), wid/2, hei/2, 0, "Hello world!");
            al_draw_bitmap(myshabmp, mysha.x, mysha.y, 0);

            al_draw_bitmap(wall, area[0][0].x, area[0][0].y, 0);
            al_draw_filled_rectangle(240, 260, 340, 340, al_map_rgba_f(0, 0, 0.5, 0.5));
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_bitmap(myshabmp);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    free(area);

    return 0;
}