#include "render.h"

#include "common.h"

void pt(const glm::vec3& p)
{
	glVertex3f(p.x, p.y, p.z);
}
void pt(const glm::vec3& p, float u, float v)
{
	glTexCoord2f(u, v);
	glVertex3f(p.x, p.y, p.z);
}
void drawDomino(float x, float y, float z)
{
	x /= 2;
	y /= 2;
	z /= 2;
	
	/*   6-----7
	 *  /|    /|
	 * 2-----3 |
	 * | |   | |
	 * | 5---|-8
	 * |/    |/
	 * 1-----4
	 *
	 * */
	
	glm::vec3 p1(-x, -y, -z);
	glm::vec3 p2(-x, y, -z);
	glm::vec3 p3(x, y, -z);
	glm::vec3 p4(x, -y, -z);
	glm::vec3 p5(-x, -y, z);
	glm::vec3 p6(-x, y, z);
	glm::vec3 p7(x, y, z);
	glm::vec3 p8(x, -y, z);
	
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	pt(p1, 0, 1);
	pt(p2, 0, 0);
	pt(p3, 0.5, 0);
	pt(p4, 0.5, 1);
	
	glNormal3f(0.0f, 0.0f, -1.0f);
	pt(p8, 0.5, 1);
	pt(p7, 0.5, 0);
	pt(p6, 1, 0);
	pt(p5, 1, 1);
	glEnd();
	
	glColor3f(0.1, 0.1, 0.1);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	pt(p5);
	pt(p6);
	pt(p2);
	pt(p1);

	glNormal3f(1.0f, 0.0f, 0.0f);
	pt(p4);
	pt(p3);
	pt(p7);
	pt(p8);

	glNormal3f(0.0f, 1.0f, 0.0f);
	pt(p2);
	pt(p6);
	pt(p7);
	pt(p3);

	glNormal3f(0.0f, -1.0f, 0.0f);
	pt(p5);
	pt(p1);
	pt(p4);
	pt(p8);
	glEnd();
}
