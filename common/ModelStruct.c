#include "ModelStruct.h"
#include <stdio.h>
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"


void MS_update_modToWorld(MS *ms)
{
    ms->modToWorld = Mult(T(ms->pos.x, ms->pos.y, ms->pos.z),Mult(ms->rot, ms->scale));
}

void MS_assign_modToWorld(MS *ms, mat4 mat)
{
    ms->modToWorld = mat;
}

void MS_draw_mod(MS *ms)
{
    glUseProgram(ms->progs[0]);
    glUniform1f(glGetUniformLocation(ms->progs[0], "kd"), ms->kd);
    glUniform1f(glGetUniformLocation(ms->progs[0], "ks"), ms->ks);
    glUniform1f(glGetUniformLocation(ms->progs[0], "ka"), ms->ka);
    glUniform1f(glGetUniformLocation(ms->progs[0], "alpha"), ms->alpha);
    glUniform1f(glGetUniformLocation(ms->progs[0], "alpha_spec"), ms->alpha_spec);
    glUniform1i(glGetUniformLocation(ms->progs[0], "useDiffTex"), ms->useDiffTex);
    glUniform1i(glGetUniformLocation(ms->progs[0], "useSpecTex"), ms->useSpecTex);
    glUniform1i(glGetUniformLocation(ms->progs[0], "useHeightTex"), ms->useHeightTex);
    glUniform1i(glGetUniformLocation(ms->progs[0], "useNormTex"), ms->useNormalTex);
	glUniform1i(glGetUniformLocation(ms->progs[0], "useAlpha"), ms->useAlpha);
    glUniformMatrix4fv(glGetUniformLocation(ms->progs[0], "mdlMatrix"), 1, GL_TRUE, ms->modToWorld.m);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ms->texes[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (ms->useDiffTex)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ms->texes[1]);
    }
    if (ms->useSpecTex)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ms->texes[2]);
    }
    if (ms->useHeightTex)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ms->texes[3]);
    }
    if (ms->useNormalTex)
    {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ms->texes[4]);
    }
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ms->texes[3]);
    DrawModel(ms->mod, ms->progs[0], "inPosition", "inNormal", "inTexCoord");
}