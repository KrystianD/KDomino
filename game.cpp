#include "game.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

#include <IL/il.h>
#include <IL/ilu.h>



#define ILUT_USE_OPENGL
#include <IL/ilut.h>

Game game;

ILuint ImageName;
void Game::init()
{
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
}
void Game::render()
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
	glVertex3f(-w, 0, -w);
	glTexCoord2f(0, 0);
	glVertex3f(w, 0, -w);
	glTexCoord2f(10, 0);
	glVertex3f(w, 0, w);
	glTexCoord2f(10, 10);
	glVertex3f(-w, 0, w);
	glTexCoord2f(0, 10);
	
	glEnd();
	
	
	
	glColor3f(1, 1, 1);
	glTranslated(0, 0, 40);
	GLUquadric *q = gluNewQuadric();
	gluSphere(q, 10, 30, 30);
	glTranslated(20, 0, 40);
	gluSphere(q, 10, 30, 30);
	gluDeleteQuadric(q);
}
