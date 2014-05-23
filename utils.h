#include "common.h"

#include <ode/ode.h>
#include <limits>

uint32_t getTicks();
void printMatrix(const glm::mat4x4& m);
dReal* glmToODE(const glm::mat4x4& m);
int loadTexture(const string& path);
