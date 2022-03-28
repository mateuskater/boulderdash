#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

#define RES_X 640
#define RES_Y 480

void testa_init(bool test, const char *objeto){
    if(test) return;
    printf("%s não pôde ser inicializado\n", objeto);
    exit(1);
}

int main(int argc, char *argv[]){
    int wid = atoi(argv[1]), hei = atoi(argv[2]), x = wid/2, y = hei/2;
    if (argv[1] == NULL || argv[2] == NULL){ // valores padrão caso tamanho não for informado
        wid = 640;
        hei = 480;
    };
        
    testa_init(al_init(), "allegro");
    testa_init(al_install_keyboard(), "teclado");
    testa_init(al_init_image_addon(), "addon de imagem");
    testa_init(al_init_primitives_addon(), "primitivos");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    testa_init(timer, "timer ");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    testa_init(queue, "queue ");

    ALLEGRO_DISPLAY* disp = al_create_display(wid, hei);
    testa_init(disp, "display ");

    ALLEGRO_FONT* font = al_create_builtin_font();
    testa_init(font, "addon de fonte ");

    ALLEGRO_BITMAP* mysha = al_load_bitmap("mysha.png");
    testa_init(mysha, "ratinho");

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

        if(al_key_down(&ks, ALLEGRO_KEY_RIGHT))
            x+=5;
        if(al_key_down(&ks, ALLEGRO_KEY_LEFT))
            x-=5;
        if(al_key_down(&ks, ALLEGRO_KEY_UP))
            y-=5;
        if(al_key_down(&ks, ALLEGRO_KEY_DOWN))
            y+=5;
        if(event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), x, y, 0, "Hello world!");
            al_draw_bitmap(mysha, x, y, 0);
            al_draw_filled_triangle(35, 350, 85, 375, x, y, al_map_rgb_f(0, 1, 0));
            al_draw_filled_rectangle(240, 260, 340, 340, al_map_rgba_f(0, 0, 0.5, 0.5));
            al_draw_circle(450, 370, 30+x, al_map_rgb_f(1, 0, 1), 2);
            al_draw_line(440, 110, 460, 210, al_map_rgb_f(1, 0, 0), 1);
            al_draw_line(500, 220, 570, 200, al_map_rgb_f(1, 1, 0), 1);
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_bitmap(mysha);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}