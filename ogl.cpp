#include "ogl.h"

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

bool oglInit()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;
	
	// error = glGetError();
	// if (error != GL_NO_ERROR)
	// {
	// fprintf(stderr, "Error initializing OpenGL! %s\n", gluErrorString(error));
	// return false;
	// }
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	return true;
}
void oglResize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glOrtho(0, width, 0, height, -100, 100);
	glViewport(0, 0, width, height);
	gluPerspective(60.0, (float)width / (float)height, 10, 10000);
	glScalef(1, 1, 1);
}
