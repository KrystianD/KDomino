#ifndef __COMMON_H__
#define __COMMON_H__

const double DOMINO_X = 3.0 / 100;
const double DOMINO_Y = 8.0 / 100;
const double DOMINO_Z = 0.5 / 100;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>

#include <IL/il.h>
#include <IL/ilu.h>
#define ILUT_USE_OPENGL
#include <IL/ilut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"

#endif
