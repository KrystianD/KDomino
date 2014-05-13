SOURCES=main.o sdl.o ogl.o camera.o render.o game.o utils.o

CFLAGS=-I.

.o.o:
	g++ -c ${CFLAGS} $< -o $@

sim: $(SOURCES)
	g++ $(SOURCES) ${CFLAGS} -lSDL -lGL -lGLU -lIL -lILU -lILUT -lode -o $@
