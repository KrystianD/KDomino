#include "game.hpp"

#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <IL/il.h>
#include <IL/ilu.h>

#include <glm/mat4x3.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define ILUT_USE_OPENGL
#include <IL/ilut.h>

const double DOMINO_X = 0.5;
const double DOMINO_Y = 8;
const double DOMINO_Z = 3;

Game game;


static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	Game *game = (Game*)data;
	
	// printf("nearCallback\n");
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	dContact contact;
	contact.surface.mode = 0;//dContactSlip2;// 0;//dContactSoftCFM;
	contact.surface.slip2 = 1;
	// friction parameter
	if (b1 == game->body || b2 == game->body)
		contact.surface.mu = dInfinity;
	else
		contact.surface.mu = 0;
	// bounce is the amount of "bouncyness".
	contact.surface.bounce = 0;
	// bounce_vel is the minimum incoming velocity to cause a bounce
	contact.surface.bounce_vel = 0.1;
	// constraint force mixing parameter
	contact.surface.soft_cfm = 0.001;
	if (int numc = dCollide(o1, o2, 1, &contact.geom, sizeof(dContact)))
	{
		dJointID c = dJointCreateContact(game->world, game->contactgroup, &contact);
		dJointAttach(c, b1, b2);
	}
}

ILuint ImageName;
void Game::init()
{
	m_camera.setPosition(0, 6, -10);
	
	ilInit();
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);
	bool t = ilLoadImage("res/grass.jpg");
	printf("%d\n", t);
	
	ilutRenderer(ILUT_OPENGL);
	
	// ilutGLBindTexImage();
	ilutGLBindMipmaps();
	//
	glEnable(GL_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	odeInit();
	
	for (int i = 0; i < 100; i++)
	{
		Domino *d = new Domino();
		dMassSetBox(&d->m, 10, DOMINO_X, DOMINO_Y, DOMINO_Z);
		
		d->body = dBodyCreate(world);
		dBodySetMass(d->body, &d->m);
		
		d->geom = dCreateBox(space, DOMINO_X, DOMINO_Y, DOMINO_Z);
		dGeomSetBody(d->geom, d->body);
		
		dBodySetPosition(d->body, i * 6, 4, 0);
		
		m_dominoes.push_back(d);
	}
}

void drawBox(float x, float y, float z);
void Game::render(float dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_GREATER);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_camera.setOglMatrix();
	
	// ilutGLBindTexImage();
	
	
	glBegin(GL_QUADS);
	
	float w = 200;
	glTexCoord2f(0, 0);
	glVertex3f(-w, 0, -w);
	glTexCoord2f(20, 0);
	glVertex3f(w, 0, -w);
	glTexCoord2f(20, 20);
	glVertex3f(w, 0, w);
	glTexCoord2f(0, 20);
	glVertex3f(-w, 0, w);
	
	glEnd();
	
	
	// const dReal *pos;
	// const dReal *R;
	// pos = dGeomGetPosition(geom);
	// R = dGeomGetRotation(geom);
	
	// glPushMatrix();
	// glTranslated(pos[0], pos[1], pos[2]);
	
	// glColor3f(1, 1, 1);
	// GLUquadric *q = gluNewQuadric();
	// gluSphere(q, 1, 30, 30);
	// gluDeleteQuadric(q);
	// glPopMatrix();
	
	vec3 pt = m_camera.getPosition();
	dBodySetPosition(body2, pt.x, pt.y, pt.z);
	
	dSpaceCollide(space, this, &nearCallback);
	dWorldQuickStep(world, dt);
	dJointGroupEmpty(contactgroup);
	
	for (int i = 0; i < 100; i++)
	{
		Domino *d = m_dominoes[i];
		
		const dReal *pos;
		const dReal *R;
		double m[16];
		pos = dGeomGetPosition(d->geom);
		R = dGeomGetRotation(d->geom);
		
		printf("A: ");
		for (int i = 0; i < 12; i++)
		{
			printf("%f, ", R[i]);
		}
		printf("\n");
		// glm::dmat4x3
		
		memset(m, 0, sizeof(double) * 16);
		memcpy(m, R, sizeof(double) * 12);
		m[15] = 1;
		
		
		glPushMatrix();
		glTranslated(pos[0], pos[1], pos[2]);
		glMultTransposeMatrixd(m);
		drawBox(DOMINO_X, DOMINO_Y, DOMINO_Z);
		glColor3f(1, 1, 1);
		glPopMatrix();
		printf("%f %f %f\n", pos[0], pos[1], pos[2]);
	}
}






void drawBox(float x, float y, float z)
{
	x /= 2;
	y /= 2;
	z /= 2;
	
	glBegin(GL_POLYGON);/* f1: front */
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(0, 1);
	glVertex3f(-x, -y, z);
	glTexCoord2f(1, 1);
	glVertex3f(x,  -y, z);
	glTexCoord2f(1, 0);
	glVertex3f(x,  -y, -z);
	glEnd();
	glBegin(GL_POLYGON);/* f2: bottom */
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(0, 1);
	glVertex3f(x,  -y, -z);
	glTexCoord2f(1, 1);
	glVertex3f(x,  y, -z);
	glTexCoord2f(1, 0);
	glVertex3f(-x, y, -z);
	glEnd();
	glBegin(GL_POLYGON);/* f3:back */
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x,  y, -z);
	glTexCoord2f(0, 1);
	glVertex3f(x,  y, z);
	glTexCoord2f(1, 1);
	glVertex3f(-x, y, z);
	glTexCoord2f(1, 0);
	glVertex3f(-x, y, -z);
	glEnd();
	glBegin(GL_POLYGON);/* f4: top */
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x,  y, z);
	glTexCoord2f(0, 1);
	glVertex3f(x,  -y, z);
	glTexCoord2f(1, 1);
	glVertex3f(-x, -y, z);
	glTexCoord2f(1, 0);
	glVertex3f(-x, y, z);
	glEnd();
	glBegin(GL_POLYGON);/* f5: left */
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-x, -y, -z);
	glTexCoord2f(0, 1);
	glVertex3f(-x, y, -z);
	glTexCoord2f(1, 1);
	glVertex3f(-x, y, z);
	glTexCoord2f(1, 0);
	glVertex3f(-x, -y, z);
	glEnd();
	glBegin(GL_POLYGON);/* f6: right */
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0, 0);
	glVertex3f(x, -y, -z);
	glTexCoord2f(0, 1);
	glVertex3f(x, -y, z);
	glTexCoord2f(1, 1);
	glVertex3f(x, y, z);
	glTexCoord2f(1, 0);
	glVertex3f(x, y, -z);
	glEnd();
}






// simulation loop
void Game::simLoop(int pause)
{
	const dReal *pos;
	const dReal *R;
	// find collisions and add contact joints
	// redraw sphere at new location
	
	
	//double worldPos = dGeomGetPosition(world);
	//    dsDrawSphere (pos,R,dGeomSphereGetRadius (geom));
}

// start simulation - set viewpoint
void Game::odeInit()
{
	dInitODE();
	// create world
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	dWorldSetGravity(world, 0, -9.8, 0);
	dWorldSetCFM(world, 1e-5);
	dCreatePlane(space, 0, 1, 0, 0);
	contactgroup = dJointGroupCreate(0);
	// create object
	// body = dBodyCreate(world);
	// geom = dCreateSphere(space, 1);
	// dMassSetSphere(&m, 1, 1);
	// dBodySetMass(body, &m);
	// dGeomSetBody(geom, body);
	// // set initial position
	// dBodySetPosition(body, 0, 10, 0);
	// // run simulation
	body2 = dBodyCreate(world);
	geom2 = dCreateSphere(space, 1);
	dMassSetSphere(&m, 200, 1);
	dBodySetMass(body2, &m);
	dGeomSetBody(geom2, body2);
	// set initial position
	
	// clean up
	// dJointGroupDestroy (contactgroup);
	// dSpaceDestroy (space);
	// dWorldDestroy (world);
	// dCloseODE();
}
