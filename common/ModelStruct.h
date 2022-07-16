// Header for model class

#ifndef MODELSTRUCT
#define MODELSTRUCT

#include "VectorUtils3.h"
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#include "loadobj.h"
#include <stdbool.h>


typedef struct MS MS;

struct MS
{
    // Mandatory
    Model *mod;
    GLuint progs[16];
    vec3 pos;
    GLfloat kd, ks, ka, alpha_spec, alpha;
    GLfloat xsize, zsize;
    GLint useRotPoint, useDiffTex, useSpecTex, useHeightTex;
    GLint useAlpha, useNormalTex;
    mat4 modToWorld;
    mat4 rot;
    mat4 scale;

    // Optional
    vec3 rotPoint;
    GLuint texes[16];

    #ifdef __cplusplus
        MS() {
            // pos = (vec3) {0.0f, 0.0f, 0.0f};
            useRotPoint = false;
        }
    #endif
};

void MS_update_modToWorld(MS *ms);
void MS_draw_mod(MS *);
void MS_assign_modToWorld(MS *ms, mat4 mat);



// int sumilum(MS);

#endif