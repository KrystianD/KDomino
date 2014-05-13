#include "camera.h"

#include <stdio.h>

#include <stdint.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace glm;

Camera::Camera()
{
}
Camera::~Camera()
{
}

void Camera::setOglMatrix()
{
	vec3 dir = getDir();
	vec3 up(0, 1, 0);
	up = glm::rotateZ(up, m_rotation.z);
	
	vec3 pt = m_position + dir;

	gluLookAt(m_position.x, m_position.y, m_position.z, pt.x, pt.y, pt.z, up.x, up.y, up.z);
}

vec3 Camera::getDir()
{
	vec3 dir(0, 0, 1);
	dir = glm::rotateX(dir, m_rotation.x);
	dir = glm::rotateY(dir, m_rotation.y);
	return dir;
}
vec3 Camera::getUp()
{
	vec3 up(0, 1, 0);
	up = glm::rotateZ(up, m_rotation.z);
	return up;
}

void Camera::goForward(float val)
{
	m_position += getDir() * val;
}
void Camera::goBackward(float val)
{
	m_position -= getDir() * val;
}
void Camera::goLeft(float val)
{
	vec3 dir = getDir();
	vec3 up = getUp();
	vec3 right = glm::cross(dir, up);
	m_position -= right * val;
}
void Camera::goRight(float val)
{
	vec3 dir = getDir();
	vec3 up = getUp();
	vec3 right = glm::cross(dir, up);
	m_position += right * val;
}
void Camera::goUp(float val)
{
	m_position += getUp() * val;
}
void Camera::goDown(float val)
{
	m_position -= getUp() * val;
}
