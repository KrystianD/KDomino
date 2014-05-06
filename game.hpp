#ifndef __GAME_H__
#define __GAME_H__

#include "camera.hpp"
#include <ode/ode.h>

#include <vector>
using namespace std;

class Domino
{
public:
	dBodyID body;
	dGeomID geom;
	dMass m;
};

class Game
{
public:
	Camera& getCamera()
	{
		return m_camera;
	}
	
	void init();
	void render(float dt);
	
	void simLoop(int pause);
	void odeInit();
	
// private:
	Camera m_camera;
	
	dWorldID world;
	dSpaceID space;
	dBodyID body, body2;
	dGeomID geom, geom2;
	dMass m;
	dJointGroupID contactgroup;

	vector<Domino*> m_dominoes;
};

extern Game game;

#endif
