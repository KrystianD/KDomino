#include "utils.h"

#include <sys/time.h>

uint32_t getTicks()
{
	timeval tv;
	gettimeofday(&tv, 0);
	uint32_t val = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return val;
}
void printMatrix(const glm::mat4x4& m)
{
	const float *R = glm::value_ptr(m);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%.2f ", R[j * 4 + i]);
		}
		printf("\n");
	}
}
dReal* glmToODE(const glm::mat4x4& m)
{
	static dReal R[12];
	R[0] = glm::row(m, 0).x;
	R[1] = glm::row(m, 0).y;
	R[2] = glm::row(m, 0).z;
	R[3] = glm::row(m, 0).w;
	
	R[4] = glm::row(m, 1).x;
	R[5] = glm::row(m, 1).y;
	R[6] = glm::row(m, 1).z;
	R[7] = glm::row(m, 1).w;
	
	R[8] = glm::row(m, 2).x;
	R[9] = glm::row(m, 2).y;
	R[10] = glm::row(m, 2).z;
	R[11] = glm::row(m, 2).w;
	return R;
}
int loadTexture(const string& path)
{
	unsigned int tex;
	ilLoadImage(path.c_str());
	
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
	                  ilGetInteger(IL_IMAGE_BPP),
	                  ilGetInteger(IL_IMAGE_WIDTH),
	                  ilGetInteger(IL_IMAGE_HEIGHT),
	                  ilGetInteger(IL_IMAGE_FORMAT),
	                  GL_UNSIGNED_BYTE,
	                  ilGetData());
	                  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return tex;
}
