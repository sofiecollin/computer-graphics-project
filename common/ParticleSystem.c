#include "ParticleSystem.h"
GLint last_used_index = 0;
unsigned int vertexBufferObjID, positionBufferObjID, colorBufferObjID;
unsigned int vertexArrayObjID;

void InitParticles(GLuint program, GLuint *tex, mat4 projMatrix, int num_of_particles)
{
    glUseProgram(program);
    GLfloat vertices[] = {-0.5f,-0.5f,0.0f,
						  -0.5f,0.5f,0.0f,
					       0.5f,-0.5f,0.0f};

    //Upload geometry to the GPU:
	//Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);

    // VBO for vertex data
	glGenBuffers(1, &vertexBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
 
	// VBO for position data
	glGenBuffers(1, &positionBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, num_of_particles*3*sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 

	// VBO for color data
	glGenBuffers(1, &colorBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, num_of_particles*4*sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0); 

	glVertexAttribDivisor(0, 0); 
	glVertexAttribDivisor(1, 1); 
	glVertexAttribDivisor(2, 1);

	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
	LoadTGATextureSimple("../Textures/star1.tga", &*tex);

    glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projMatrix.m);

}

GLint FirstUnusedIndex(Particle *container, int num_of_particles)
{
    for (GLint i = last_used_index; i < num_of_particles; i++){
        if (container[i].life <= 0){
            last_used_index = i;
            return i;
        }
    } 
    for (GLint i = 0; i<last_used_index; i++){
        if (container[i].life <= 0){
            last_used_index = i; 
            return i;
        }
    }
    last_used_index = 0;
    return last_used_index;
}

void CreateNewParticle(Particle *particle, vec3 start_pos)
{
	float random1 = ((rand()%100)-50)/20.0f;
	float random2 = ((rand()%100)-50)/20.0f;
	float random3 = ((rand()%100)-50)/5.0f;

    particle->life = 5.0f + random3;

	vec3 random_pos = (vec3){random1,0,random2};
    particle->position = VectorAdd(start_pos,random_pos);

	vec3 dir = (vec3){0,0.5,0};
	particle->velocity = dir;

	particle->color = (vec4){1,0.8,0,0.5};
}

void StartRendering(GLuint program)
{
	glUseProgram(program);
	glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
	//For transparency and glow
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vertexArrayObjID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void StopRendering()
{
	//Default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	glUseProgram(0);
}

void UpdateAndDrawParticles(GLfloat delta, GLint num_new_particles, Particle *container, int num_of_particles, vec3 start_pos, mat4 camMatrix, GLuint program, GLuint tex)
{
    for(int i=0; i<num_new_particles;i++){
		GLint ind = FirstUnusedIndex(container,num_of_particles);
        CreateNewParticle(&container[ind],start_pos);
	}
	
	GLfloat *position_data = malloc(sizeof(GLfloat) * 3 * num_of_particles);
	GLfloat *color_data = malloc(sizeof(GLfloat) * 4 * num_of_particles);

	int count = 0;

	for(int i = 0; i<num_of_particles;i++){
		Particle *p= &container[i];
		if(p->life > 0.0f){
			p->life -= delta;
			if(p->life > 0.0f){
				// Update position, color and mdlMatrix
				p->position.x += p->velocity.x * delta;
				p->position.y += p->velocity.y * delta; 
				p->position.z += p->velocity.z * delta;
				
				p->color.y -= 0.01;
				p->color.w -= 0.003;

				// Fill buffers
				position_data[3*count+0] = p->position.x;
				position_data[3*count+1] = p->position.y;
				position_data[3*count+2] = p->position.z;

				color_data[4*count+0] = p->color.x;
				color_data[4*count+1] = p->color.y;
				color_data[4*count+2] = p->color.z;
				color_data[4*count+3] = p->color.w;
			}
			count += 1;
		}
	}
	

	StartRendering(program);

	mat4 mdlMatrix = getmdlMatrix(camMatrix, start_pos);
    glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix.m);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, count*3*sizeof(GLfloat), position_data, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, count*4*sizeof(GLfloat), color_data, GL_STREAM_DRAW);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, count);

	StopRendering();

}

mat4 getmdlMatrix(mat4 camMatrix, vec3 particlePos)
{
		mat4 rot = Ry(-M_PI/2);
		mat4 scale = S(8.0f,8.0f,1.0f);
		vec3 trans = (vec3){-210,-36,17};
		mat4 modelView = Mult(rot,scale);

		mat4 mdlMatrix = Mult(camMatrix, Mult(T(particlePos.x+trans.x,particlePos.y+trans.y,particlePos.z+trans.z), modelView));
		return mdlMatrix;
}