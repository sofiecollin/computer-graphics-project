# set this variable to the director in which you saved the common files
commondir = ../common/

all : Project Hampus

Project : Project.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)ModelStruct.c $(commondir)ParticleSystem.c
	gcc -Wall -o Project -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES Project.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)ModelStruct.c $(commondir)ParticleSystem.c -lXt -lX11 -lGL -lm

Hampus : Hampus.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)ModelStruct.c $(commondir)ParticleSystem.c
	gcc -Wall -o Hampus -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES Hampus.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c $(commondir)ModelStruct.c $(commondir)ParticleSystem.c -lXt -lX11 -lGL -lm


clean :
	rm Project
	rm Hampus
