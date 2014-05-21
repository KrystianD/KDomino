#include "common.h"

#include <ode/ode.h>

void printMatrix(const glm::mat4x4& m);
dReal* glmToODE(const glm::mat4x4& m);
int loadTexture(const string& path);
