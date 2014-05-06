#ifndef __SDL_H__
#define __SDL_H__

bool sdlInit(int width, int height);
void sdlClose();

bool sdlResize(int width, int height);

void sdlSwap();

#endif
