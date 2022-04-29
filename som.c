#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "som.h"

t_sons carrega_sons(void){
    t_sons sons;
    sons.diamante = al_load_sample("./resources/sounds/diamante.ogg");
    sons.terra = al_load_sample("./resources/sounds/terra.ogg");
    sons.musica = al_load_sample("./resources/sounds/bg_music.ogg");
    sons.morreu = al_load_sample("./resources/sounds/lose.wav");
    sons.vitoria = al_load_sample("./resources/sounds/win.wav");
    return sons;
}

void destroi_audio(t_sons *sons){
    al_destroy_sample(sons->diamante);
    al_destroy_sample(sons->terra);
    al_destroy_sample(sons->musica);
    al_destroy_sample(sons->morreu);
    al_destroy_sample(sons->vitoria);
}