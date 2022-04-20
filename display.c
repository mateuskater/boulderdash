#include <stdio.h>
#include <allegro5/allegro.h>
#include "bdash.h"
#include "display.h"

ALLEGRO_DISPLAY *inicializa_tela(int w, int h){
	ALLEGRO_DISPLAY *disp = NULL;
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    disp = al_create_display(w, h);
    if (!disp) {
		fprintf(stderr, "Cannot create allegro display!");
		return NULL;
	}

    return disp;
}

ALLEGRO_TRANSFORM setup_transform(ALLEGRO_DISPLAY *disp){
    ALLEGRO_TRANSFORM t;
    const float scale_factor_x = ((float)al_get_display_width(disp)) / DEFAULT_WIDTH;
	const float scale_factor_y = ((float)al_get_display_height(disp)) / DEFAULT_HEIGHT;
    al_identity_transform(&t);
	al_scale_transform(&t, scale_factor_x, scale_factor_y);
	al_use_transform(&t);
    return t;
}