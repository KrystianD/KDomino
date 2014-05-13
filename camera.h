#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "common.h"

class Camera
{
public:
	Camera();
	~Camera();
	
	glm::vec3 getPosition() const { return m_position; }
	void setPosition(float x, float y, float z)
	{
		m_position = glm::vec3(x, y, z);
	}
	void setPosition(const glm::vec3& v)
	{
		setPosition(v.x, v.y, v.z);
	}

	glm::vec3 getRotation() const { return m_rotation; }
	void setRotation(const glm::vec3& v) { m_rotation = v; }

	glm::vec3 getDir();
	glm::vec3 getUp();
	
	void goForward(float val);
	void goBackward(float val);
	void goLeft(float val);
	void goRight(float val);
	void goUp(float val);
	void goDown(float val);

	void setOglMatrix();
private:
	glm::vec3 m_position, m_rotation;
};

#endif
