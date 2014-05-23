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
	uint texId;
	bool dis, touched;
	float idleTime;
	double initAngle;
	dReal R[16];
	
	void setPosition(double x, double y, double z, float ang)
	{
		glm::mat4x4 m;
		m = glm::rotate(m, ang, glm::vec3(0, 1, 0));
		
		dBodySetPosition(body, x, 0 + DOMINO_Y / 2, z);
		dBodySetRotation(body, glmToODE(m));
	}
	glm::vec3 getPosition()
	{
		const dReal *pos = dGeomGetPosition(geom);
		return glm::vec3(pos[0], pos[1], pos[2]);
	}
	
	double* getMatrix()
	{
		static double m[16];
		if (!dis)
		{
			const dReal *pos = dGeomGetPosition(geom);
			const dReal *rot = dGeomGetRotation(geom);
			
			memset(R, 0, sizeof(double) * 16);
			for (int i = 0; i < 12; i++)
				R[i] = rot[i];
			R[3] = pos[0];
			R[7] = pos[1];
			R[11] = pos[2];
			R[15] = 1;
		}
		return R;
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
	
	void drawStop();
	void drawStart(int x, int y);
	void drawMouse(int x, int y);
	void select(int x, int y);
	void clear();
	
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
	vector<uint> m_dominoTextures;
	obj_scene_data m_dominoObj;
	
	
	//drawing
	glm::vec3 m_drawLastPt;
	bool m_drawing;

	glm::vec3 unproject(int x, int y);
	void drawProcessPoint(int x, int y);

	void rolloverDomino(Domino* d);
};

extern Game game;

#endif
