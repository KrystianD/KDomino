SOURCES=main.o sdl.o ogl.o camera.o render.o game.o utils.o 3rd/obj_parser.o 3rd/list.o 3rd/string_extra.o

CFLAGS=-I. -ggdb -g

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

sim: $(SOURCES)
	g++ $(SOURCES) $(CFLAGS) -lSDL -lGL -lGLU -lIL -lILU -lILUT -lode -lpthread -o $@
