#ifndef AUDIO_H
#define AUDIO_H
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

typedef struct t_sons{
    ALLEGRO_SAMPLE *diamante, *terra, *musica;
}t_sons;

t_sons carrega_sons();

#endif