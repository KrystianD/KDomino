#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <cmath>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "common.h"
#include "ogl.h"
#include "sdl.h"
#include "camera.h"
#include "render.h"
#include "game.h"

// #include <kutils.h>
// #include "Color.h"


// #include <string>
// using namespace std;

int width = 600;
int height = 600;

void render();

#include "3rd/obj_parser.h"
int main()
{
	// chdir("res");
	// obj_scene_data m_dominoObj;
	// parse_obj_scene(&m_dominoObj, "beretta.obj");
	// printf("ASD\n");
	
	if (!sdlInit(width, height))
		return 1;
	if (!oglInit())
		return 1;
		
	oglResize(width, height);
	
	game.init();
	
	bool quit = false;
	uint32_t lastSendTime = getTicks();
	bool rotating = false;
	bool drawing = false;
	while (!quit)
	{
		uint8_t *keystate = SDL_GetKeyState(0);
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_LCTRL:
					// SDL_ShowCursor(0);
					// SDL_WM_GrabInput(SDL_GRAB_ON);
					// rotating = true;
					break;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_LCTRL:
					// SDL_ShowCursor(1);
					// SDL_WM_GrabInput(SDL_GRAB_OFF);
					// rotating = false;
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				SDL_MouseMotionEvent ev = event.motion;
				
				if (rotating)
				{
					static bool first = true;
					if (first)
					{
						first = false;
						break;
					}
					float xrel = -(float)ev.xrel / 100.0f;
					float yrel = (float)ev.yrel / 100.0f;
					
					game.getCamera().setRotation(game.getCamera().getRotation() + glm::vec3(yrel, xrel, 0));
				}
				if (drawing)
				{
					game.drawMouse(ev.x, ev.y);
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				SDL_MouseButtonEvent ev = event.button;
				switch (ev.button)
				{
				case 1:
					// rotating = false;
					// SDL_ShowCursor(1);
					// SDL_WM_GrabInput(SDL_GRAB_OFF);
					break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_MouseButtonEvent ev = event.button;
				switch (ev.button)
				{
				case 1:
					if (keystate[SDLK_LCTRL])
					{
						drawing = true;
					}
					else
					{
						SDL_ShowCursor(0);
						// SDL_WM_GrabInput(SDL_GRAB_ON);
						rotating = true;
					}
					break;
				case 5: // scroll down
					break;
				case 4: // scroll up
					break;
				}
			}
			else if (event.type == SDL_VIDEORESIZE)
			{
				SDL_ResizeEvent ev = event.resize;
				width = ev.w;
				height = ev.h;
				oglResize(ev.w, ev.h);
				sdlResize(ev.w, ev.h);
			}
		}
		
		if (!(SDL_GetMouseState(0, 0) & SDL_BUTTON(1)))
		{
			rotating = false;
			drawing = false;
			SDL_ShowCursor(1);
			SDL_WM_GrabInput(SDL_GRAB_OFF);
		}
		
		float camSpeed = 0.006;
		if (keystate[SDLK_w])
		{
			game.getCamera().goForward(camSpeed);
		}
		if (keystate[SDLK_s])
		{
			game.getCamera().goBackward(camSpeed);
		}
		if (keystate[SDLK_a])
		{
			game.getCamera().goLeft(camSpeed);
		}
		if (keystate[SDLK_d])
		{
			game.getCamera().goRight(camSpeed);
		}
		if (keystate[SDLK_q])
		{
			game.getCamera().goUp(camSpeed);
		}
		if (keystate[SDLK_e])
		{
			game.getCamera().goDown(camSpeed);
		}
		
		float dt;
		static uint32_t lastTicks = 0;
		if (lastTicks == 0) lastTicks = getTicks();
		uint32_t diff = getTicks() - lastTicks;
		if (diff > 0)
		{
			float dt = (float)diff / 1000.0f;
			game.render(dt);
			lastTicks = getTicks();
		}
		sdlSwap();
	}
	
	sdlClose();
	
	return 0;
}
