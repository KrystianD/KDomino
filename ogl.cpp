#include "ogl.h"

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

bool oglInit()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	return true;
}
void oglResize(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(60.0, (float)width / (float)height, 0.0001, 100);
}
