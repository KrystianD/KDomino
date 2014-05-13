#include "game.h"

#include "utils.h"
#include "render.h"

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
	
	int numc = dCollide(o1, o2, 1, &contact.geom, sizeof(dContact));
	if (numc)
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
	iluInit();
	ilutInit();
	uint id;
	glEnable(GL_TEXTURE);
	
	ilGenImages(1, &id);
	ilBindImage(id);
	
	m_grassTex = loadTexture("res/grass.jpg");
	m_dominoTex = loadTexture("res/d.jpg");
	
	odeInit();
	
	for (int i = 0; i < 100; i++)
	{
		Domino *d = new Domino();
		dMassSetBox(&d->m, 10, DOMINO_X, DOMINO_Y, DOMINO_Z);
		
		d->body = dBodyCreate(world);
		dBodySetMass(d->body, &d->m);
		
		d->geom = dCreateBox(space, DOMINO_X, DOMINO_Y, DOMINO_Z);
		dGeomSetBody(d->geom, d->body);

		float z = sinf(i * 0.7) * 3;
		float x = i * 6;
		d->setPosition(x, 0, z, 0);
		
		m_dominoes.push_back(d);
	}
}
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

void Game::render(float dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_camera.setOglMatrix();
	
	glBindTexture(GL_TEXTURE_2D, m_grassTex);
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
	
	glm::vec3 pt = m_camera.getPosition();
	dBodySetPosition(body2, pt.x, pt.y, pt.z);
	
	dSpaceCollide(space, this, &nearCallback);
	dWorldQuickStep(world, dt);
	dJointGroupEmpty(contactgroup);
	
	for (int i = 0; i < 100; i++)
	{
		Domino *d = m_dominoes[i];
		
		double *M = d->getMatrix();

		glBindTexture(GL_TEXTURE_2D, m_dominoTex);
		glPushMatrix();
		glMultTransposeMatrixd(M);
		drawBox(DOMINO_X, DOMINO_Y, DOMINO_Z);
		glColor3f(1, 1, 1);
		glPopMatrix();
	}
}

