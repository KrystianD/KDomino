#ifndef __GAME_H__
#define __GAME_H__

#include "camera.hpp"

class Game
{
public:
	Camera& getCamera() { return m_camera; }

	void init();
	void render();

private:
	Camera m_camera;
};

extern Game game;

#endif
