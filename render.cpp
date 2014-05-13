#include "render.h"

#include "common.h"

// #include "game.h"

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
