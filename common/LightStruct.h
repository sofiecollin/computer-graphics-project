// Header for light class

#ifndef LIGHTSTRUCT
#define MODELSTRUCT

#include "VectorUtils3.h"
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include "loadobj.h"
#include <stdbool.h>

typedef struct LS LS;

struct LS
{
    GLint isDirectional;
    GLint isSpot;
    GLfloat intensity;
    GLfloat radii;
    GLfloat kd;
    GLfloat ks;
    GLfloat ka;
    GLfloat alpha;
    GLfloat kl;
    GLfloat kq;
    vec3 dir_pos;
    vec3 color;

};






#endif