#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "som.h"

t_sons carrega_sons(void){
    t_sons sons;
    sons.diamante = al_load_sample("./resources/sounds/diamante.ogg");
    return sons;
}