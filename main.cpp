#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

#include "ogl.h"
#include "sdl.h"
#include "camera.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

// #include <kutils.h>
// #include "Color.h"

uint32_t getTicks()
{
	timeval tv;
	gettimeofday(&tv, 0);
	uint32_t val = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return val;
}

#include <string>
using namespace std;

int width = 600;
int height = 600;
Camera c;

void render();

int main()
{
	if (!sdlInit(width, height))
		return 1;
	if (!oglInit())
		return 1;
		
	oglResize(width, height);
	
	bool quit = false;
	uint32_t lastSendTime = getTicks();
	while (!quit)
	{
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
				case SDLK_UP:
					break;
				case SDLK_DOWN:
					break;
				case SDLK_LEFT:
					break;
				case SDLK_RIGHT:
					break;
				}
				
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				SDL_MouseMotionEvent ev = event.motion;
				
				printf("%d %d\n", ev.y, ev.x);
				printf("%d %d\n", ev.xrel, ev.yrel);
				
				float xrel = -(float)ev.xrel / 100.0f;
				float yrel = -(float)ev.yrel / 100.0f;
				
				c.setRotation(c.getRotation() + vec3(yrel, xrel, 0));
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				SDL_MouseButtonEvent ev = event.button;
				switch (ev.button)
				{
				case 1:
					break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_MouseButtonEvent ev = event.button;
				float newScale, dv;
				int evy;
				switch (ev.button)
				{
				case 1:
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

		uint8_t *keystate = SDL_GetKeyState(0);
		if (keystate[SDLK_w])
		{
			c.goForward(1);
		}
		else if (keystate[SDLK_s])
		{
			c.goBackward(1);
		}
		else if (keystate[SDLK_a])
		{
			c.goLeft(1);
		}
		else if (keystate[SDLK_d])
		{
			c.goRight(1);
		}
		else if (keystate[SDLK_q])
		{
			c.goUp(1);
		}
		else if (keystate[SDLK_e])
		{
			c.goDown(1);
		}
		
		render();
		sdlSwap();
	}
	
	sdlClose();
	
	return 0;
}

void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	c.setOglMatrix();
	// glTranslatef(x, y, 0);
	// glScalef(scale, scale, scale);
	
	glColor3f(1, 1, 1);
	glTranslated(0, 0, 40);
	GLUquadric *q = gluNewQuadric();
	gluSphere(q, 10, 30, 30);
	glTranslated(20, 0, 40);
	gluSphere(q, 10, 30, 30);
	gluDeleteQuadric(q);
	
	c.setOglMatrix();
}
