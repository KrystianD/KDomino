#include "game.h"

#include "utils.h"
#include "render.h"

#include <cmath>

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
	// m_dominoTex = loadTexture("res/d.jpg");
	
	// chdir("res");
	// parse_obj_scene(&m_dominoObj, "domino.obj");
	// chdir("..");
	
	for (int i = 0; i < m_dominoObj.material_count; i++)
	{
		obj_material *m = m_dominoObj.material_list[i];
		
		char* path = m->texture_filename;
		printf("load tex: %s\n", path);
		m_dominoTex = loadTexture("res/d.jpg");
	}
	
	for (int i = 1; i <= 23; i++)
	{
		char path[100];
		sprintf(path, "textures/domino%d.jpg", i);
		uint tex = loadTexture(path);
		m_dominoTextures.push_back(tex);
	}
	
	odeInit();
	
	// for (int i = 0; i < 100; i++)
	// {
		// Domino *d = new Domino();
		// dMassSetBox(&d->m, 10, DOMINO_X, DOMINO_Y, DOMINO_Z);
		
		// d->body = dBodyCreate(world);
		// dBodySetMass(d->body, &d->m);
		
		// d->geom = dCreateBox(space, DOMINO_X, DOMINO_Y, DOMINO_Z);
		// dGeomSetBody(d->geom, d->body);
		
		// float ang = (float)i / 100.0f * 3.14 * 2;
		// float z = sinf(ang) * 90;
		// float x = cosf(ang) * 90;
		
		// if (rand() % 10 >= 5)
			// d->setPosition(x, 0, z, -ang - 3.14 / 2);
		// else
			// d->setPosition(x, 0, z, -ang + 3.14 / 2);
			
		// d->texId = rand() % 23;
		// m_dominoes.push_back(d);
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
	glScalef(-1, 1, -1);
	
	glEnable(GL_TEXTURE_2D);
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
	
	// printf("%f %f %f\r\n", m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);
	
	glm::vec3 pt = m_camera.getPosition();
	dBodySetPosition(body2, pt.x, pt.y, pt.z);
	
	dSpaceCollide(space, this, &nearCallback);
	dWorldQuickStep(world, dt);
	dJointGroupEmpty(contactgroup);
	
	for (int i = 0; i < m_dominoes.size(); i++)
	{
		Domino *d = m_dominoes[i];
		
		glBindTexture(GL_TEXTURE_2D, m_dominoTextures[d->texId]);
		glPushMatrix();
		glMultTransposeMatrixd(d->getMatrix());
		drawDomino(DOMINO_X, DOMINO_Y, DOMINO_Z);
		
		// printf("a %d\n", m_dominoObj.face_count);
		// obj_face **f = m_dominoObj.face_list;
		// for (int i = 0; i < m_dominoObj.face_count; i++)
		// {
		// obj_face *f = m_dominoObj.face_list[i];
		
		// glBegin(GL_TRIANGLES);
		
		// for (int j = 0; j < f->vertex_count; j++)
		// {
		// int v = f->vertex_index[j];
		// int t = f->texture_index[j];
		
		// obj_vector *vect = m_dominoObj.vertex_list[v];
		// obj_vector *uv = m_dominoObj.vertex_texture_list[t];
		
		// glTexCoord2f(uv->e[0], uv->e[1]);
		// glVertex3f(vect->e[0], vect->e[1], vect->e[2]);
		// }
		
		// glEnd();
		
		// // printf("%d\n", f->vertex_count);
		// }
		
		
		
		glColor3f(1, 1, 1);
		glPopMatrix();
	}
	// float winZ;
	// glReadPixels(250, 250, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	
	// printf("winz %f\r\n", winZ);
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

	if(d < Epsilon)
	{
		intersectionDistance = glm::dot(planeOrig - orig, planeNormal) / d;
		return true;
	}

	return false;
}

void Game::drawMouse(int x, int y)
{
	// printf("%d %d\r\n", x, y);
	
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
	  x,
	  view[2] - y,
	  0,
	  model,
	  proj,
	  view,
	  &ox,
	  &oy,
	  &oz);

	glm::vec3 camPos = m_camera.getPosition();
	glm::vec3 dir (ox, oy, oz);
	glm::vec3 camRay = glm::normalize(dir - camPos);

	float dist;
	intersectRayPlane(camPos, camRay, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), dist);
	// printf("ox %5.2f oy %5.2f oz %5.2f - %f\n", ox, oy, oz, dist);

	glm::vec3 pt = camPos + camRay * dist;

	if (glm::distance(m_drawLastPt, pt) > 3)
	{
		Domino *d = new Domino();

		glm::vec3 dir = pt - m_drawLastPt;
		dir = glm::normalize(dir);

		float ang = atan2f(dir.x, dir.z);

		printf("%.2f %.2f %.2f\n", dir.x, dir.z, ang /3.14*180);

		dMassSetBox(&d->m, 10, DOMINO_X, DOMINO_Y, DOMINO_Z);
		
		d->body = dBodyCreate(world);
		dBodySetMass(d->body, &d->m);
		
		d->geom = dCreateBox(space, DOMINO_X, DOMINO_Y, DOMINO_Z);
		dGeomSetBody(d->geom, d->body);
		
		d->setPosition(pt.x, pt.y, pt.z, ang);
			
		d->texId = rand() % 23;
		m_dominoes.push_back(d);

		m_drawLastPt = pt;
	}

	// m_dominoes[0]->setPosition(pt.x,pt.y,pt.z,0);

	// printf("%f %f %f\r\n", ox, oy, oz);
}
