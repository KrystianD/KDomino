SOURCES=main.o sdl.o ogl.o camera.o render.o game.o utils.o

CFLAGS=-I. -ggdb -g

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

domino: $(SOURCES)
	g++ $(SOURCES) $(CFLAGS) -lSDL -lGL -lGLU -lIL -lILU -lILUT -lode -lpthread -o $@
