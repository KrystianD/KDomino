#include "sdl.h"

#include <SDL/SDL.h>

void sdl_die(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
	SDL_Quit();
}

bool sdlInit(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		sdl_die("Unable to initialize SDL");
		return false;
	}
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_ShowCursor(0);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	
	if (!sdlResize(width, height))
		return false;
		
	return true;
}
void sdlClose()
{
	SDL_Quit();
}

bool sdlResize(int width, int height)
{
	const SDL_VideoInfo* info = NULL;
	int bpp = 0;
	
	info = SDL_GetVideoInfo();
	if (!info)
	{
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		return false;
	}
	
	bpp = info->vfmt->BitsPerPixel;
	
	SDL_SetVideoMode(width, height, bpp, SDL_OPENGL | SDL_RESIZABLE);
	
	return true;
}

void sdlSwap()
{
	SDL_GL_SwapBuffers();
}
