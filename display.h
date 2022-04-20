#ifndef DISPLAY_H
#define DISPLAY_H

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 368

ALLEGRO_DISPLAY *inicializa_tela(int w, int h);
ALLEGRO_TRANSFORM setup_transform(ALLEGRO_DISPLAY *disp);

#endif