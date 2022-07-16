#ifndef PARTICLESYSTEM
#define PARTICLESYSTEM

#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"


typedef struct Particle
{
    vec3 position, velocity;
    vec4 color;
    GLfloat life,size;

}Particle;

void InitParticles(GLuint program, GLuint *tex, mat4 projMatrix, int num_of_particles);

GLint FirstUnusedIndex(Particle *container, int num_of_particles);

void CreateNewParticle(Particle *particle, vec3 start_pos);

mat4 getmdlMatrix(mat4 camMatrix, vec3 startPos);

void StartRendering(GLuint program);

void StopRendering();

void UpdateAndDrawParticles(GLfloat delta, GLint num_new_particles, Particle *container, int num_of_particles, vec3 start_pos, mat4 camMatrix, GLuint program, GLuint tex);

#endif