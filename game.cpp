#include "game.h"

#include "utils.h"
#include "render.h"

#include <cmath>

Game game;

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	Game *game = (Game*)data;
	
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	
	const int N = 1;
	dContact contact[N];
	
	int numc = dCollide(o1, o2, N, &(contact[0].geom), sizeof(dContact));
	for (int i = 0; i < numc; i++)
	{
		contact[i].surface.mode = dContactApprox1;//0;//dContactSoftCFM;//dContactSlip2;// 0;//dContactSoftCFM;
		if ((b1) == 0 || (b2) == 0)
			contact[i].surface.mu = 1;
		else
			contact[i].surface.mu = 0.0001;
			
		contact[i].surface.soft_cfm = 0.001;
		
		dJointID c = dJointCreateContact(game->world, game->contactgroup, &contact[i]);
		dJointAttach(c, b1, b2);
	}
	
	static int q = 0;
	static uint32_t l = 0;
	if (getTicks() - l >= 1000)
	{
		l = getTicks();
		printf("%d\n", q);
		q = 0;
	}
	q++;
}

void Game::init()
{
	m_camera.setPosition(0, 0.3, -0.5);
	
	ilInit();
	iluInit();
	ilutInit();
	uint id;
	glEnable(GL_TEXTURE);
	
	ilGenImages(1, &id);
	ilBindImage(id);
	
	m_grassTex = loadTexture("res/grass.jpg");
	// m_dominoTex = loadTexture("res/d.jpg");
	
	// chdir("res");
	// parse_obj_scene(&m_dominoObj, "domino.obj");
	// chdir("..");
	
	// for (int i = 0; i < m_dominoObj.material_count; i++)
	// {
	// obj_material *m = m_dominoObj.material_list[i];
	
	// char* path = m->texture_filename;
	// printf("load tex: %s\n", path);
	// }
	m_dominoTex = loadTexture("a.jpg");
	
	for (int i = 1; i <= 23; i++)
	{
		char path[100];
		sprintf(path, "textures/domino%d.jpg", i);
		uint tex = loadTexture(path);
		m_dominoTextures.push_back(tex);
	}
	
	odeInit();
	
	// for (int i = 0; i < 30; i++)
	// {
	// Domino *d = new Domino();
	// dMassSetBox(&d->m, 10, DOMINO_X, DOMINO_Y, DOMINO_Z);
	
	// d->body = dBodyCreate(world);
	// dBodySetMass(d->body, &d->m);
	
	// d->geom = dCreateBox(space, DOMINO_X, DOMINO_Y, DOMINO_Z);
	// dGeomSetBody(d->geom, d->body);
	
	// float ang = (float)i / 30.0f * 3.14 * 2;
	// float z = sinf(ang) * 0.20;
	// float x = cosf(ang) * 0.20;
	
	// if (rand() % 10 >= 5)
	// d->setPosition(x, 0, z, -ang);
	// else
	// d->setPosition(x, 0, z, -ang + 3.14);
	
	// d->texId = rand() % 23;
	// m_dominoes.push_back(d);
	
	// d->dis = false;
	// dBodySetData(d->body, d);
	
	// // dBodyDisable(d->body);
	// }
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
	geom2 = dCreateSphere(space, 0.01);
	dMassSetSphere(&m, 200, 1);
	dBodySetMass(body2, &m);
	dGeomSetBody(geom2, body2);
	
	dWorldSetQuickStepNumIterations(world, 64);
}

void Game::render(float dt)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_camera.setOglMatrix();
	glScalef(-1, 1, -1);
	
	glPushMatrix();
	glTranslatef(m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);
	
	// draw sphere
	int n;
	double a;
	double b;
	const double div = 10;
	// glDepthFunc(GL_GREATER);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_dominoTex);
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_STRIP);
	double PI = M_PI;
	double R = 10;
	double H = 0, K = 0;
	
	for (b = 0; b <= 180 - div; b += div)
	{
		for (a = 0; a <= 360 - div; a += div)
		{
			float X, Y, Z, U, V;
			
			X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
			Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
			Z = R * cos((b) / 180 * PI) ;
			V = (2 * b) / 360;
			U = (a) / 360;
			n++;
			
			U *= 10;
			V *= 10;
			glTexCoord2f(U, V);
			glVertex3f(X, Y, Z);
			
			X = R * sin((a) / 180 * PI) * sin((b + div) / 180 * PI) - H;
			Y = R * cos((a) / 180 * PI) * sin((b + div) / 180 * PI) + K;
			Z = R * cos((b + div) / 180 * PI) ;
			V = (2 * (b + div)) / 360;
			U = (a) / 360;
			n++;
			U *= 10;
			V *= 10;
			glTexCoord2f(U, V);
			glVertex3f(X, Y, Z);
			
			X = R * sin((a + div) / 180 * PI) * sin((b) / 180 * PI) - H;
			Y = R * cos((a + div) / 180 * PI) * sin((b) / 180 * PI) + K;
			Z = R * cos((b) / 180 * PI) ;
			V = (2 * b) / 360;
			U = (a + div) / 360;
			n++;
			U *= 10;
			V *= 10;
			glTexCoord2f(U, V);
			glVertex3f(X, Y, Z);
			
			X = R * sin((a + div) / 180 * PI) * sin((b + div) / 180 * PI) - H;
			Y = R * cos((a + div) / 180 * PI) * sin((b + div) / 180 * PI) + K;
			Z = R * cos((b + div) / 180 * PI) ;
			V = (2 * (b + div)) / 360;
			U = (a + div) / 360;
			n++;
			
			U *= 10;
			V *= 10;
			glTexCoord2f(U, V);
			glVertex3f(X, Y, Z);
			
		}
	}
	
	glEnd();
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glPopMatrix();
	
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_grassTex);
	glBegin(GL_QUADS);
	
	float w = 2;
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
	
	double simstep = 0.001;
	double dt2 = dt;
	int nrofsteps = (int)ceilf(dt2 / simstep);
	for (int i = 0; i < nrofsteps; i++)
	{
		dSpaceCollide(space, this, &nearCallback);
		dWorldQuickStep(world, simstep);
		dJointGroupEmpty(contactgroup);
	}
	
	for (int i = 0; i < m_dominoes.size(); i++)
	{
		Domino *d = m_dominoes[i];
		
		if (d->dis)
			continue;
			
		const dReal *d1;
		
		if (dBodyIsEnabled(d->body))
		{
			d1 = dBodyGetAngularVel(d->body);
			
			glm::vec3 p = d->getPosition();
			
			double q = abs(d1[0] * d1[0] + d1[1] * d1[1] + d1[2] * d1[2]) / 3.0;
			// printf("q %f\n", q);
			if (q > 0.05)
			{
				d->touched = true;
			}
			if (d->touched)
			{
				if (q < 5 || p.y < DOMINO_Y / 2 - 0.005)
				{
					d->idleTime += dt;
					if (d->idleTime >= 1)
					{
						d->dis = true;
						dBodyDisable(d->body);
						dBodyDestroy(d->body);
						dGeomDestroy(d->geom);
					}
				}
				else
				{
					d->idleTime = 0;
				}
			}
		}
	}
	
	for (int i = 0; i < m_dominoes.size(); i++)
	{
		Domino *d = m_dominoes[i];
		
		glBindTexture(GL_TEXTURE_2D, m_dominoTextures[d->texId]);
		glPushMatrix();
		glMultTransposeMatrixd(d->getMatrix());
		drawDomino(DOMINO_X, DOMINO_Y, DOMINO_Z);
		
		glColor3f(1, 1, 1);
		glPopMatrix();
	}
}

#include <limits>
template <typename genType>
GLM_FUNC_QUALIFIER bool intersectRayPlane
(
  genType const & orig, genType const & dir,
  genType const & planeOrig, genType const & planeNormal,
  typename genType::value_type & intersectionDistance
)
{
	typename genType::value_type d = glm::dot(dir, planeNormal);
	typename genType::value_type Epsilon = std::numeric_limits<typename genType::value_type>::epsilon();
	
	if (d < Epsilon)
	{
		intersectionDistance = glm::dot(planeOrig - orig, planeNormal) / d;
		return true;
	}
	
	return false;
}

void Game::drawStop()
{
	m_drawing = false;
}
void Game::drawStart(int x, int y)
{
	drawProcessPoint(x, y);
}
void Game::drawMouse(int x, int y)
{
	drawProcessPoint(x, y);
}
void Game::select(int x, int y)
{
	glm::vec3 pt = unproject(x, y);
	// pt.y = DOMINO_Y / 2;
	
	for (int i = 0; i < m_dominoes.size(); i++)
	{
		Domino *d = m_dominoes[i];

		if (d->dis)
			continue;

		glm::vec3 p = d->getPosition();
		
		if (glm::distance(p, pt) < DOMINO_Y + 0.02)
		{
			rolloverDomino(d);
			break;
		}
	}
	
	printf("ox %5.2f oy %5.2f oz %5.2f - %f\n", pt.x, pt.y, pt.z, 0);
}
void Game::clear()
{
	for (int i = 0 ; i < m_dominoes.size(); i++)
	{
		Domino *d = m_dominoes[i];
		if (!d->dis)
		{
			dBodyDisable(d->body);
			dBodyDestroy(d->body);
			dGeomDestroy(d->geom);
		}
		delete d;
	}
	m_dominoes.clear();
}

glm::vec3 Game::unproject(int x, int y)
{
	double model[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	double proj[16];
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	int view[4];
	glGetIntegerv(GL_VIEWPORT, view);
	
	// float winZ;
	// glReadPixels(x, 600-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	
	double ox, oy, oz;
	gluUnProject(
	  x, view[3] - y, 0,
	  model, proj, view,
	  &ox, &oy, &oz);
	  
	glm::vec3 camPos = m_camera.getPosition();
	glm::vec3 dir(ox, oy, oz);
	glm::vec3 camRay = glm::normalize(dir - camPos);
	
	float dist;
	intersectRayPlane(camPos, camRay, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), dist);
	
	glm::vec3 pt = camPos + camRay * dist;
	
	return pt;
}
void Game::drawProcessPoint(int x, int y)
{
	// printf("ox %5.2f oy %5.2f oz %5.2f - %f\n", pt.x, pt.y, pt.z, dist);
	glm::vec3 pt = unproject(x, y);
	
	if (!m_drawing)
	{
		m_drawLastPt = pt;
		m_drawing = true;
	}
	
	while (glm::distance(m_drawLastPt, pt) > 0.03)
	{
		Domino *d = new Domino();
		
		glm::vec3 dir = pt - m_drawLastPt;
		dir = glm::normalize(dir);
		
		m_drawLastPt += dir * 0.03f;
		
		float ang = atan2f(dir.x, dir.z);
		
		dMassSetBox(&d->m, 10.0, DOMINO_X, DOMINO_Y, DOMINO_Z);
		
		d->body = dBodyCreate(world);
		dBodySetMass(d->body, &d->m);
		
		d->geom = dCreateBox(space, DOMINO_X, DOMINO_Y, DOMINO_Z);
		dGeomSetBody(d->geom, d->body);
		
		d->setPosition(m_drawLastPt.x, m_drawLastPt.y, m_drawLastPt.z, ang + (rand() % 100 > 50 ? 3.14 : 0));
		d->initAngle = ang;
		
		d->texId = rand() % 23;
		
		d->dis = false;
		d->touched = false;
		d->idleTime = 0;
		dBodySetData(d->body, d);
		
		m_dominoes.push_back(d);
	}
}
void Game::rolloverDomino(Domino* d)
{
	float ang = d->initAngle;
	
	glm::vec3 v(0, 0, 0.01);
	v = glm::rotateY(v, ang);
	
	dBodyAddForce(d->body, v.x, v.y, v.z);
}
