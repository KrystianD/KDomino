SOURCES=main.cpp sdl.cpp ogl.cpp camera.cpp
sim: $(SOURCES)
	g++ $(SOURCES) -lSDL -lGL -lGLU -I. -o $@
