CFLAGS = -Wall -g
LDLIBS = $$(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 --cflags --libs)

OBJS = game.o display.o lista.o init_sprites.o som.o funcoes.o

all: game
game: $(OBJS)

funcoes.o: funcoes.c game.h
	gcc -c funcoes.c $(LDLIBS) $(CFLAGS)
som.o: som.c som.h
	gcc -c som.c $(LDLIBS) $(CFLAGS)
display.o: display.c display.h
	gcc -c display.c $(LDLIBS) $(CFLAGS)
lista.o: lista.c lista.h
	gcc -c lista.c $(LDLIBS) $(CFLAGS)
init_sprites.o: init_sprites.c init_sprites.h
	gcc -c init_sprites.c $(LDLIBS) $(CFLAGS)
game.o: game.c game.h
	gcc -c game.c $(LDLIBS) $(CFLAGS)

clean:
	-rm -f $(OBJS)

purge: clean
	-rm -f game