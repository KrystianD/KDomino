SOURCES=main.cpp sdl.cpp ogl.cpp camera.cpp render.cpp game.cpp
sim: $(SOURCES)
	g++ $(SOURCES) -lSDL -lGL -lGLU -lIL -lILU -lILUT -lode -I. -o $@
