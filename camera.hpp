#include <glm/vec3.hpp>
using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();
	
	vec3 getPosition() const { return m_position; }
	void setPosition(float x, float y, float z)
	{
		m_position = vec3(x, y, z);
	}
	void setPosition(const vec3& v)
	{
		setPosition(v.x, v.y, v.z);
	}

	vec3 getRotation() const { return m_rotation; }
	void setRotation(const vec3& v) { m_rotation = v; }

	vec3 getDir();
	vec3 getUp();
	
	void goForward(float val);
	void goBackward(float val);
	void goLeft(float val);
	void goRight(float val);
	void goUp(float val);
	void goDown(float val);

	void setOglMatrix();
private:
	vec3 m_position, m_rotation;
};
