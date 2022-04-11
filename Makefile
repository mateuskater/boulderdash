CFLAGS = -Wall -g
LDLIBS = $$(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --cflags --libs)

objs = game.o display.o lista_pedras.o init_sprites.o

all: game
game: $(objs)

clean:
	-rm -f $(objs)

purge: clean
	-rm -f game
