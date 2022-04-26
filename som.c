#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "som.h"

t_sons carrega_sons(void){
    t_sons sons;
    sons.diamante = al_load_sample("./resources/sounds/diamante.ogg");
    sons.terra = al_load_sample("./resources/sounds/terra.ogg");
    sons.musica = al_load_sample("./resources/sounds/bg_music.ogg");
    return sons;
}