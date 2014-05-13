#ifndef __GAME_H__
#define __GAME_H__

#include <ode/ode.h>
#include "common.h"
#include "camera.h"

#include "3rd/obj_parser.h"

class Domino
{
public:
	dBodyID body;
	dGeomID geom;
	dMass m;

	void setPosition(double x, double y, double z, float ang)
	{
		glm::mat4x4 m;
		m = glm::rotate(m, ang, glm::vec3(0, 1, 0));
		
		dBodySetPosition(body, x, 0 + DOMINO_Y / 2, z);
		dBodySetRotation(body, glmToODE(m));
	}

	double* getMatrix()
	{
		static double m[16];
		const dReal *pos = dGeomGetPosition(geom);
		const dReal *R = dGeomGetRotation(geom);

		memset(m, 0, sizeof(double) * 16);
		memcpy(m, R, sizeof(double) * 12);
		m[3] = pos[0];
		m[7] = pos[1];
		m[11] = pos[2];
		m[15] = 1;
		return m;
	}
};

class Game
{
public:
	Camera& getCamera()
	{
		return m_camera;
	}
	
	void init();
	void odeInit();
	void render(float dt);
	
// private:
	Camera m_camera;
	
	dWorldID world;
	dSpaceID space;
	dBodyID body, body2;
	dGeomID geom, geom2;
	dMass m;
	dJointGroupID contactgroup;

	vector<Domino*> m_dominoes;

	uint m_grassTex, m_dominoTex;
	obj_scene_data m_dominoObj;
};

extern Game game;

#endif