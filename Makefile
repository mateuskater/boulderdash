CFLAGS = -Wall
LDLIBS = $$(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --cflags --libs)

objs = hello.o

all: hello

clean:
	-rm -f $(objs)

purge: clean
	-rm -f hello
