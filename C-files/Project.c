// Hampus' and Sofie's project for TSBB15.
// Note, this runs on Linux only (tested on Linux Mint and Virtual Box)
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "ModelStruct.h"
#include "LightStruct.h"
#include "ParticleSystem.h"
#include <math.h>


// Declare functions
void updatePosition();
void updateCameraPos(int, int);
void timer(int);
void initModels();
void initLights();
void updateLights();
void uploadLights(LS list[160], int);
int collisionDetection(MS*);
void resolveCollision(MS*);
void collisionHandling();
void initCollisionList();
void drawPillars();
void drawLeftRightWalls();
void drawFrames();
void drawPanes();
void resolveAOBCollision();
void drawLamps();



// Declaration of globals
// Declare projection Matrix
mat4 projectionMatrix;

// Declaration of Models
Model *stoolModel;
Model *cubeModel;
Model *unicornModel;
Model *HorsieModel;
Model *churchModel;
Model *containerModel;
Model *fireplaceModel, *fireplaceBacksideModel, *frameModel, *paneModel;
Model *pillarColumnModel;
Model *pillarFoundationModel;
Model *floorFloorModel, *roofModel;
Model *leftWallModel, *frontWallModel, *backWallModel;
Model *skybox;
Model *groundModel;
Model *reaperModel;
Model *whiteModel, *bronzeModel, *steelModel, *chainModel;

// Programs
GLuint program, prog_particle, prog_skybox;

// Textures
GLuint texGrass, texConc, texContainer, texContainerSpec, texFireplace, texFire, texSkybox; 
GLuint texFireplaceBackside, texGround, texBronze, texSteel, texWhite;
GLuint texFrame, texFrame_H, texFrame_R, texPane; 
GLuint texColumn, texColumnHeight, texColumnRough, texColumnSpec, texPillarFoundation, texPillarFoundationSpec;
GLuint texFloorFloor, texFloorFloorSpec, texFloorFloorHeight, texFloorFloorNormal;
GLuint texFrontWall, texFrontWall_H, texFrontWall_R;
GLuint texRoof, texRoof_H;


// Declaration of all objects
MS unicorn, bronze, steel, white, chain;
MS stool1;
MS stool2;
MS stool3;
MS stool4;
MS cube;
MS church;
MS fireplace, fireplaceBackside;
MS container, frame, pane;
MS ground;
MS pillarColumn;
MS pillarFoundation;
MS floorFloor;
MS leftWall, frontWall, backWall, roof;
MS reaper;
MS DummyCol1,DummyCol2,DummyCol3,DummyCol4,DummyCol5, DummyCol6;
MS* collisionList[150];
int nrCollisionObjects = 7;

// Declaration of all Lights
LS light1;
LS light2;
LS light3;
LS light4;
LS fire;
LS candle1;
LS candle2;

// Initiation of globals
#define num_of_particles 5000
int num_new_particles = 50;
GLfloat particle_delta = 0.05f;
float phi = 90;
float theta = 0;
int mousex = 300, mousey = 300;
int xmax = 0;
int zmax = 0;
vec3 p = {0.0f, -10.0f, 0.0f};
vec3 l = {1.0f, 0.0f, 0.0f};
vec3 v = {0.0f, 2.0f, 0.0f};
vec3 fire_startpos = (vec3){0.0f,0.0f,0.0f};
Particle particle_container[num_of_particles];
float size = 1;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	printError("GL inits");
	

	// Load and compile shader
	program = loadShaders("../Shaders/main.vert", "../Shaders/main.frag");
	prog_particle = loadShaders("../Shaders/particle.vert","../Shaders/particle.frag");
	prog_skybox = loadShaders("../Shaders/skybox.vert","../Shaders/skybox.frag");
	glUseProgram(program);
	printError("init shader");

	// Unchangeable matricis
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 3000.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(prog_skybox);
	glUniformMatrix4fv(glGetUniformLocation(prog_skybox, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(program);

	// Textures
	LoadTGATextureSimple("../Textures/maskros512.tga", &texGrass);
	LoadTGATextureSimple("../Textures/conc.tga", &texConc);
	LoadTGATextureSimple("../Textures/StoneFireplace.tga", &texFireplace);
	LoadTGATextureSimple("../Textures/Marble.tga", &texFireplaceBackside);
	LoadTGATextureSimple("../Textures/12.tga", &texPane);

	LoadTGATextureSimple("../Textures/container.tga", &texContainer);
	LoadTGATextureSimple("../Textures/container2_specular.tga", &texContainerSpec);
	
	LoadTGATextureSimple("../Textures/Wood/Wood.tga", &texFrame);
	LoadTGATextureSimple("../Textures/Wood/Wood.tga", &texFrame_H);
	LoadTGATextureSimple("../Textures/Wood/Wood.tga", &texFrame_R);

	LoadTGATextureSimple("../Textures/Column/Column_B.tga", &texColumn);
	LoadTGATextureSimple("../Textures/Column/Column_H.tga", &texColumnHeight);
	LoadTGATextureSimple("../Textures/Column/Column_R.tga", &texColumnRough);
	LoadTGATextureSimple("../Textures/Column/Column_S.tga", &texColumnSpec);
	LoadTGATextureSimple("../Textures/RockyRock/Rocky.tga", &texPillarFoundation);
	LoadTGATextureSimple("../Textures/RockyRock/Rocky_H.tga", &texPillarFoundationSpec);


	LoadTGATextureSimple("../Textures/Floor/PavingFloor_D.tga", &texFloorFloor);
	LoadTGATextureSimple("../Textures/Floor/PavingFloor_S.tga", &texFloorFloorSpec);
	LoadTGATextureSimple("../Textures/Floor/PavingFloor_H.tga", &texFloorFloorHeight);
	LoadTGATextureSimple("../Textures/Floor/PavingFloor_N.tga", &texFloorFloorNormal);

	LoadTGATextureSimple("../Textures/Stone_Wall/Stone_Wall.tga", &texFrontWall);
	LoadTGATextureSimple("../Textures/Stone_Wall/Stone_Wall_H.tga", &texFrontWall_H);
	LoadTGATextureSimple("../Textures/Stone_Wall/Stone_Wall_R.tga", &texFrontWall_R);

	LoadTGATextureSimple("../Textures/SkyBox513.tga", &texSkybox);
	LoadTGATextureSimple("../Textures/Ground.tga", &texGround);

	LoadTGATextureSimple("../Textures/Roof/StoneRoof.tga", &texRoof);
	LoadTGATextureSimple("../Textures/Roof/StoneRoof_H.tga", &texRoof_H);

	LoadTGATextureSimple("../Textures/Bronze.tga", &texBronze);
	LoadTGATextureSimple("../Textures/Steel.tga", &texSteel);
	LoadTGATextureSimple("../Textures/White.tga", &texWhite);

	
	glUseProgram(prog_skybox);
	glUniform1i(glGetUniformLocation(prog_skybox, "tex"), 0);

	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "texMain"),0);
	glUniform1i(glGetUniformLocation(program, "material.diffuse"),1);
	glUniform1i(glGetUniformLocation(program, "material.specular"),2);
	glUniform1i(glGetUniformLocation(program, "texHeight"),3);
	glUniform1i(glGetUniformLocation(program, "material.normal"),4);

	// Load Models
	stoolModel = LoadModelPlus("../Objects/stool.obj");
	cubeModel = LoadModelPlus("../Objects/Cube.obj");
	unicornModel = LoadModelPlus("../Objects/Unicorn.obj");
	cubeModel = LoadModelPlus("../Objects/groundsphere.obj");
	churchModel = LoadModelPlus("../Objects/Church.obj");
	containerModel = LoadModelPlus("../Objects/container.obj");
	pillarColumnModel = LoadModelPlus("../Objects/PillarColumn.obj");
	pillarFoundationModel = LoadModelPlus("../Objects/PillarFoundation.obj");
	floorFloorModel = LoadModelPlus("../Objects/Floor2.obj");
	frontWallModel = LoadModelPlus("../Objects/FrontWall.obj");
	leftWallModel = LoadModelPlus("../Objects/LeftWall.obj");
	backWallModel = LoadModelPlus("../Objects/BackWall.obj");
	fireplaceModel = LoadModelPlus("../Objects/Fireplace.obj");
	fireplaceBacksideModel = LoadModelPlus("../Objects/FireplaceBackside.obj");
	skybox = LoadModelPlus("../Objects/skybox2.obj");
	roofModel = LoadModelPlus("../Objects/Roof2.obj");
	frameModel = LoadModelPlus("../Objects/Frame.obj");
	paneModel = LoadModelPlus("../Objects/Glass.obj");
	groundModel = LoadModelPlus("../Objects/Ground.obj");
	reaperModel = LoadModelPlus("../Objects/Reaper.obj");
	bronzeModel = LoadModelPlus("../Objects/Bronze.obj");
	steelModel = LoadModelPlus("../Objects/Steel.obj");
	whiteModel = LoadModelPlus("../Objects/White.obj");
	chainModel = LoadModelPlus("../Objects/Chain.obj");

	

	// Other
	glutWarpPointer(mousex,mousey);
	initLights();
	initModels();
	initCollisionList();

	// Init particles
	InitParticles(prog_particle, &texFire, projectionMatrix, num_of_particles);

}



void display(void)
{
	
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("pre display");

	updatePosition();
	collisionHandling();
	resolveAOBCollision();

	glUseProgram(program);
	
	//Send useful stuff to the shaders
	mat4 camMatrix = lookAtv(p, VectorAdd(p,l), v);
	mat4 skycamMatrix = camMatrix;
	skycamMatrix.m[3] = 0;
	skycamMatrix.m[7] = -1;
	skycamMatrix.m[11] = 0;

	glUseProgram(prog_skybox);
	glUniformMatrix4fv(glGetUniformLocation(prog_skybox, "camMatrix"), 1, GL_TRUE, skycamMatrix.m);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texSkybox);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);
	glUniform1f(glGetUniformLocation(program, "camx"), p.x);
	glUniform1f(glGetUniformLocation(program, "camy"), p.y);
	glUniform1i(glGetUniformLocation(program, "camz"), p.z);

	// glUseProgram(prog_skybox);
	// glUniformMatrix4fv(glGetUniformLocation(prog_skybox, "camMatrix"), 1, GL_TRUE, camMatrix.m);

	// Update objects
	MS_update_modToWorld(&floorFloor);
	MS_update_modToWorld(&unicorn);
	MS_update_modToWorld(&pillarFoundation);
	MS_update_modToWorld(&pillarColumn);
	MS_update_modToWorld(&container);
	MS_update_modToWorld(&frontWall);
	MS_update_modToWorld(&leftWall);
	MS_update_modToWorld(&backWall);
	MS_update_modToWorld(&fireplace);
	MS_update_modToWorld(&fireplaceBackside);
	MS_update_modToWorld(&roof);
	MS_update_modToWorld(&frame);
	MS_update_modToWorld(&pane);
	MS_update_modToWorld(&ground);
	MS_update_modToWorld(&reaper);
	MS_update_modToWorld(&bronze);
	MS_update_modToWorld(&steel);
	MS_update_modToWorld(&white);
	MS_update_modToWorld(&chain);
	

	// Draw SkyBox
	glUseProgram(prog_skybox);
	glDisable(GL_DEPTH_TEST);
	DrawModel(skybox, prog_skybox, "in_Position", NULL, "in_TexCoord");
	glEnable(GL_DEPTH_TEST);

	// Draw objects
	glUseProgram(program);
	MS_draw_mod(&ground);
	MS_draw_mod(&reaper);
	drawPanes();
	MS_draw_mod(&container);
	drawLeftRightWalls();
	MS_draw_mod(&floorFloor);
	MS_draw_mod(&frontWall);
	MS_draw_mod(&backWall);
	MS_draw_mod(&fireplace);
	MS_draw_mod(&fireplaceBackside);
	drawFrames();
	MS_draw_mod(&roof);
	drawPillars();

	drawLamps();
	
	
	// Update and draw particles
	UpdateAndDrawParticles(particle_delta, num_new_particles, particle_container, num_of_particles, fire_startpos, camMatrix, prog_particle, texFire); 
	
	printf("\n\npos.x: %f\npos.y: %f\npos.z: %f\n", p.x, p.y, p.z);
	
	printError("display 2");
	glutSwapBuffers();
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1200, 1000);
	glutCreateWindow ("Project");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(updateCameraPos);

	glutMainLoop();
	exit(0);
}

void drawLeftRightWalls()
{
	vec3 wallPos = leftWall.pos;
	mat4 wallRot = leftWall.rot;
	vec3 newWallPos = wallPos;
	mat4 newWallRot = wallRot;

	MS_draw_mod(&leftWall);
	
	newWallPos.x = newWallPos.x-119.5;
	newWallPos.z = newWallPos.z + 21;
	leftWall.pos = newWallPos;
	newWallRot = Mult(Ry(M_PI), wallRot);
	leftWall.rot = newWallRot;

	MS_update_modToWorld(&leftWall);
	MS_draw_mod(&leftWall);

	leftWall.pos = wallPos;
	leftWall.rot = wallRot;
	MS_update_modToWorld(&leftWall);
}



void initModels()
{
	//Church
	church.pos = (vec3){0, 0, -1};
	church.mod = churchModel;
	church.texes[0] = texConc;
	church.scale = IdentityMatrix();
	church.rot = IdentityMatrix();
	church.progs[0] = program;
	church.useDiffTex = false;
	church.useSpecTex = false;
	church.kd = 0.30;
	church.ks = 0.5;
	church.ka = 0.1;
	church.alpha_spec = 32.0;

	

	// Floor
	floorFloor.pos = (vec3){0, 0, 0};
	floorFloor.mod = floorFloorModel;
	floorFloor.texes[0] = texFloorFloor;
	floorFloor.texes[1] = texFloorFloor;
	floorFloor.texes[2] = texFloorFloorSpec;
	floorFloor.texes[3] = texFloorFloorHeight;
	floorFloor.texes[4] = texFloorFloorNormal;
	floorFloor.scale = IdentityMatrix();
	floorFloor.rot = IdentityMatrix();
	floorFloor.progs[0] = program;
	floorFloor.useDiffTex = true;
	floorFloor.useSpecTex = true;
	floorFloor.useHeightTex = true;
	floorFloor.useNormalTex = false;
	floorFloor.kd = 1.0;
	floorFloor.ks = 1.0;
	floorFloor.ka = 0.2;
	floorFloor.alpha_spec = 12.0;

	// FrontWall
	frontWall.pos = (vec3){0, 0, 0};
	frontWall.mod = frontWallModel;
	frontWall.texes[0] = texFrontWall;
	frontWall.texes[1] = texFrontWall;
	frontWall.texes[2] = texFrontWall_R;
	frontWall.texes[3] = texFrontWall_H;
	frontWall.scale = IdentityMatrix();
	frontWall.rot = IdentityMatrix();
	frontWall.progs[0] = program;
	frontWall.useDiffTex = true;
	frontWall.useSpecTex = false;
	frontWall.useHeightTex = true;
	frontWall.kd = 1.0;
	frontWall.ks = 1.0;
	frontWall.ka = 0.1;
	frontWall.alpha_spec = 12.0;

	// BackWall
	backWall.pos = (vec3){-119.5, 0, 21};
	backWall.mod = backWallModel;
	backWall.texes[0] = texFrontWall;
	backWall.texes[1] = texFrontWall;
	backWall.texes[2] = texFrontWall_R;
	backWall.texes[3] = texFrontWall_H;
	backWall.scale = IdentityMatrix();
	backWall.rot = Ry(M_PI);
	backWall.progs[0] = program;
	backWall.useDiffTex = true;
	backWall.useSpecTex = false;
	backWall.useHeightTex = true;
	backWall.kd = 1.0;
	backWall.ks = 1.0;
	backWall.ka = 0.1;
	backWall.alpha_spec = 12.0;

	// leftWall
	leftWall.pos = (vec3){0, 0, 0};
	leftWall.mod = leftWallModel;
	leftWall.texes[0] = texFrontWall;
	leftWall.texes[1] = texFrontWall;
	leftWall.texes[2] = texFrontWall_R;
	leftWall.texes[3] = texFrontWall_H;
	leftWall.scale = IdentityMatrix();
	leftWall.rot = IdentityMatrix();
	leftWall.progs[0] = program;
	leftWall.useDiffTex = true;
	leftWall.useSpecTex = false;
	leftWall.useHeightTex = true;
	leftWall.kd = 1.0;
	leftWall.ks = 1.0;
	leftWall.ka = 0.1;
	leftWall.alpha_spec = 12.0;

	// Container
	container.pos = (vec3){-100, -30, 8};
	container.mod = containerModel;
	container.texes[0] = texContainer;
	container.texes[1] = texContainer;
	container.texes[2] = texContainerSpec;
	container.scale = S(5.0,5.0,5.0);
	container.rot = IdentityMatrix();
	container.progs[0] = program;
	container.useDiffTex = true;
	container.useSpecTex = true;
	container.useHeightTex = false;
	container.kd = 1.0;
	container.ks = 0.0;
	container.ka = 0.5;
	container.alpha_spec = 32.0;
	container.xsize = 7.0;
	container.zsize = 7.0;

	// Bronze
	bronze.pos = (vec3){0.0, 20.0, 10.0};
	bronze.mod = bronzeModel;
	bronze.texes[0] = texBronze;
	bronze.texes[1] = texBronze;
	bronze.scale = IdentityMatrix();
	bronze.rot = IdentityMatrix();
	bronze.progs[0] = program;
	bronze.useDiffTex = true;
	bronze.useSpecTex = false;
	bronze.useHeightTex = false;
	bronze.kd = 1.0;
	bronze.ks = 0.5;
	bronze.ka = 0.1;
	bronze.alpha_spec = 32.0;

	// Steel
	steel.pos = (vec3){0.0, 20.0, 10.0};
	steel.mod = steelModel;
	steel.texes[0] = texSteel;
	steel.texes[1] = texSteel;
	steel.scale = IdentityMatrix();
	steel.rot = IdentityMatrix();
	steel.progs[0] = program;
	steel.useDiffTex = true;
	steel.useSpecTex = false;
	steel.useHeightTex = false;
	steel.kd = 1.0;
	steel.ks = 0.5;
	steel.ka = 0.1;
	steel.alpha_spec = 32.0;

	// White
	white.pos = (vec3){0.0, 20.0, 10.0};
	white.mod = whiteModel;
	white.texes[0] = texConc;
	white.texes[1] = texConc;
	white.scale = IdentityMatrix();
	white.rot = IdentityMatrix();
	white.progs[0] = program;
	white.useDiffTex = true;
	white.useSpecTex = false;
	white.useHeightTex = false;
	white.kd = 1.0;
	white.ks = 0.5;
	white.ka = 0.1;
	white.alpha_spec = 32.0;

	// Chain
	chain.pos = (vec3){0.0, 20.0, 10.0};
	chain.mod = chainModel;
	chain.texes[0] = texSteel;
	chain.texes[1] = texSteel;
	chain.scale = IdentityMatrix();
	chain.rot = IdentityMatrix();
	chain.progs[0] = program;
	chain.useDiffTex = true;
	chain.useSpecTex = false;
	chain.useHeightTex = false;
	chain.kd = 1.0;
	chain.ks = 0.5;
	chain.ka = 0.1;
	chain.alpha_spec = 32.0;

	// Reaper
	reaper.pos = (vec3){0, 0, 0};
	reaper.mod = reaperModel;
	reaper.texes[0] = texGrass;
	reaper.scale = IdentityMatrix();
	reaper.rot = IdentityMatrix();
	reaper.progs[0] = program;
	reaper.useDiffTex = true;
	reaper.useSpecTex = true;
	reaper.useHeightTex = false;
	reaper.kd = 1.0;
	reaper.ks = 0.0;
	reaper.ka = 0.0;
	reaper.alpha_spec = 32.0;


	// frame
	frame.pos = (vec3){0, 0, 0};
	frame.mod = frameModel;
	frame.texes[0] = texFrame;
	frame.texes[1] = texFrame;
	frame.texes[3] = texFrame_H;
	frame.scale = IdentityMatrix();
	frame.rot = IdentityMatrix();
	frame.progs[0] = program;
	frame.useDiffTex = true;
	frame.useSpecTex = false;
	frame.useHeightTex = false;
	frame.kd = 1.0;
	frame.ks = 0.2;
	frame.ka = 0.1;
	frame.alpha_spec = 12.0;

	// Pane
	pane.pos = (vec3){0, 0, 0};
	pane.mod = paneModel;
	pane.texes[0] = texPane;
	pane.scale = IdentityMatrix();
	pane.rot = IdentityMatrix();
	pane.progs[0] = program;
	pane.useDiffTex = false;
	pane.useSpecTex = false;
	pane.useHeightTex = false;
	pane.useAlpha = true;
	pane.alpha = 0.4;
	pane.kd = 0.5;
	pane.ks = 0.4;
	pane.ka = 0.4;
	pane.alpha_spec = 12.0;

	// Roof
	roof.pos = (vec3){0, 0, -0.2};
	roof.mod = roofModel;
	roof.texes[0] = texRoof;
	roof.texes[1] = texRoof;
	roof.texes[2] = texRoof_H;
	roof.texes[3] = texRoof_H;
	roof.scale = S(1.0f, 1.0f, 1.003f);
	roof.rot = IdentityMatrix();
	roof.progs[0] = program;
	roof.useDiffTex = true;
	roof.useSpecTex = true;
	roof.useHeightTex = true;
	roof.kd = 0.5;
	roof.ks = 0.3;
	roof.ka = 0.0;
	roof.alpha_spec = 32.0;

	// Ground
	ground.pos = (vec3){0, -20, -15.0};
	ground.mod = groundModel;
	ground.texes[0] = texGround;
	ground.texes[1] = texGround;
	ground.scale = IdentityMatrix();
	ground.rot = IdentityMatrix();
	ground.progs[0] = program;
	ground.useDiffTex = true;
	ground.useSpecTex = false;
	ground.useHeightTex = false;
	ground.kd = 0.2;
	ground.ks = 0.1;
	ground.ka = 0.1;
	ground.alpha_spec = 16.0;

	// Fireplace
	fireplace.pos = (vec3){-120, 1.5, 21};
	fireplace.mod = fireplaceModel;
	fireplace.texes[0] = texFireplace;
	fireplace.scale = IdentityMatrix();
	fireplace.rot = Ry(M_PI);
	fireplace.progs[0] = program;
	fireplace.useDiffTex = false;
	fireplace.useSpecTex = false;
	fireplace.useHeightTex = false;
	fireplace.kd = 1.0;
	fireplace.ks = 0.0;
	fireplace.ka = 0.5;
	fireplace.alpha_spec = 32.0;

	// FireplaceBackside
	fireplaceBackside.pos = (vec3){-120, 1.5, 21};
	fireplaceBackside.mod = fireplaceBacksideModel;
	fireplaceBackside.texes[0] = texFireplaceBackside;
	fireplaceBackside.scale = IdentityMatrix();
	fireplaceBackside.rot = Ry(M_PI);
	fireplaceBackside.progs[0] = program;
	fireplaceBackside.useDiffTex = false;
	fireplaceBackside.useSpecTex = false;
	fireplaceBackside.useHeightTex = false;
	fireplaceBackside.kd = 1.0;
	fireplaceBackside.ks = 0.0;
	fireplaceBackside.ka = 0.5;
	fireplaceBackside.alpha_spec = 32.0;

	// Cube
	cube.pos = (vec3){0, 0, -1};
	cube.mod = cubeModel;
	cube.texes[0] = texConc;
	cube.scale = IdentityMatrix();
	cube.rot = IdentityMatrix();
	cube.progs[0] = program;
	cube.useDiffTex = 0;
	cube.useSpecTex = 0;

	// Unicorn
	unicorn.pos = (vec3){-130, -31.5, 0};
	unicorn.mod = unicornModel;
	unicorn.texes[0] = texConc;
	unicorn.scale = S(10, 10, 10);
	unicorn.rot = Ry(1.8);
	unicorn.progs[0] = program;
	unicorn.useDiffTex = false;
	unicorn.useSpecTex = false;
	unicorn.useHeightTex = false;
	unicorn.kd = 0.30;
	unicorn.ks = 0.5;
	unicorn.ka = 0.1;
	unicorn.alpha_spec = 32.0;
	unicorn.xsize = 30.0;
	unicorn.zsize = 10.0;

	// PillarFoundation
	pillarFoundation.pos = (vec3){0, 0, 0};
	pillarFoundation.mod = pillarFoundationModel;
	pillarFoundation.texes[0] = texPillarFoundation;
	pillarFoundation.texes[1] = texPillarFoundation;
	pillarFoundation.texes[2] = texPillarFoundationSpec;
	pillarFoundation.scale = IdentityMatrix();
	pillarFoundation.rot = IdentityMatrix();
	pillarFoundation.progs[0] = program;
	pillarFoundation.useDiffTex = true;
	pillarFoundation.useSpecTex = true;
	pillarFoundation.useHeightTex = false;
	pillarFoundation.kd = 0.30;
	pillarFoundation.ks = 0.5;
	pillarFoundation.ka = 0.3;
	pillarFoundation.alpha_spec = 32.0;

	// PillarColumn
	pillarColumn.pos = (vec3){0, 0, 0};
	pillarColumn.mod = pillarColumnModel;
	pillarColumn.texes[0] = texColumn;
	pillarColumn.texes[1] = texColumnRough;
	pillarColumn.texes[2] = texColumnHeight;
	pillarColumn.texes[3] = texColumnHeight;
	pillarColumn.scale = IdentityMatrix();
	pillarColumn.rot = IdentityMatrix();
	pillarColumn.progs[0] = program;
	pillarColumn.useDiffTex = true;
	pillarColumn.useSpecTex = true;
	pillarColumn.useHeightTex = false;
	pillarColumn.kd = 1.0;
	pillarColumn.ks = 1.0;
	pillarColumn.ka = 0.25;
	pillarColumn.alpha_spec = 64.0;


	// Stool 1
	stool1.pos = (vec3){0, 0, 10};
	stool1.mod = stoolModel;
	stool1.texes[0] = texConc;
	stool1.scale = IdentityMatrix();
	stool1.rot = IdentityMatrix();
	stool1.progs[0] = program;
	stool1.useDiffTex = 0;
	stool1.useSpecTex = 0;

	// Stool 2
	stool2.pos = (vec3){0, 2.4, 2.6};
	stool2.mod = stoolModel;
	stool2.texes[0] = texConc;
	stool2.scale = IdentityMatrix();
	stool2.rot = IdentityMatrix();
	stool2.progs[0] = program;
	stool2.useDiffTex = 0;
	stool2.useSpecTex = 0;

	// Dummies
	DummyCol1.pos = (vec3) {0.0, 0.0, 58.0};
	DummyCol2.pos = (vec3) {-60.0, 0.0, 58.0};
	DummyCol3.pos = (vec3) {-120.0, 0.0, 58.0};
	DummyCol4.pos = (vec3) {-0.0, 0.0, -35.0};
	DummyCol5.pos = (vec3) {-60.0, 0.0, -35.0};
	DummyCol6.pos = (vec3) {-120.0, 0.0, -35.0};

	DummyCol1.xsize = 10.0;
	DummyCol1.zsize = 10.0;
	DummyCol2.xsize = 10.0;
	DummyCol2.zsize = 10.0;
	DummyCol3.xsize = 10.0;
	DummyCol3.zsize = 10.0;
	DummyCol4.xsize = 10.0;
	DummyCol4.zsize = 10.0;
	DummyCol5.xsize = 10.0;
	DummyCol5.zsize = 10.0;
	DummyCol6.xsize = 10.0;
	DummyCol6.zsize = 10.0;

}

void updateLights()
{

}

void initCollisionList()
{
	collisionList[0] = &container;
	collisionList[1] = &DummyCol1;
	collisionList[2] = &DummyCol2;
	collisionList[3] = &DummyCol3;
	collisionList[4] = &DummyCol4;
	collisionList[5] = &DummyCol5;
	collisionList[6] = &DummyCol6;

}

void initLights()
{
	// Light1
	light1.color = (vec3){0.1, 0.0, 0.3};
	light1.isDirectional = 0;
	light1.dir_pos = (vec3){50, -3.0, 70.0};
	light1.intensity = 1.0;
	light1.kl = 0.00045;
	light1.kq = 0.00075;

	// Light2
	light2.color = (vec3){0.0, 0.0, 0.0};
	light2.isDirectional = 0;
	light2.dir_pos = (vec3){0, 0, 10};
	light2.intensity = 1.0;
	light2.kl = 0.000045;
	light2.kq = 0.000075;

	// Light3
	light3.color = (vec3){0.0, 0.0, 0.0};
	light3.isDirectional = 0;
	light3.dir_pos = (vec3){-98.0f, 1.0f, -15.0f};
	light3.intensity = 1;
	light3.kl = 0.000045;
	light3.kq = 0.000075;

	// Light4, fire in fireplace
	light4.color = (vec3){0.0, 0.0, 0.0};
	light4.isDirectional = 0;
	light4.dir_pos = (vec3){-152.0f, 50.0f, 51.0f};
	light4.intensity = 1.0;
	light4.kl = 0.000045;
	light4.kq = 0.000075;

	// fire, fireplace
	fire.color = (vec3){0.945f, 0.488f, 0.047};
	fire.isDirectional = 0;
	fire.dir_pos = (vec3){-217.0f, 0.5f, 13.8f};
	fire.intensity = 1.0;
	fire.kl = 0.00045;
	fire.kq = 0.000075;

	// candle1, candle1place
	candle1.color = (vec3){0.945f, 0.488f, 0.047};
	candle1.isDirectional = 0;
	candle1.dir_pos = (vec3){0, 28, 7};
	candle1.intensity = 1.0;
	candle1.kl = 0.045;
	candle1.kq = 0.0075;

	// candle1, candle2place
	candle2.color = (vec3){0.945f, 0.488f, 0.047};
	candle2.isDirectional = 0;
	candle2.dir_pos = (vec3){-120, 28, 7};
	candle2.intensity = 1.0;
	candle2.kl = 0.045;
	candle2.kq = 0.0075;

	

	int nrLights = 5;
	LS list[160] = {candle1, candle2, light1, light4, fire};
	uploadLights(list, nrLights);
	glUniform1i(glGetUniformLocation(program, "nrLights"),nrLights);
}

void uploadLights(LS list[160], int nrLights)
{
	vec3 colors[160];
	vec3 dir_pos[160];
	GLint isDirectional[160];
	GLfloat intensity[160];
	GLfloat kl[160];
	GLfloat kq[160];

	for (int i = 0; i < nrLights; i++)
	{
		colors[i] = list[i].color;
		dir_pos[i] = list[i].dir_pos;
		isDirectional[i] = list[i].isDirectional;
		intensity[i] = list[i].intensity;
		kl[i] = list[i].kl;
		kq[i] = list[i].kq;
	}

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 160, &dir_pos[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 160, &colors[0].x);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 160, isDirectional);
	glUniform1fv(glGetUniformLocation(program, "intensity"), 160, intensity);
	glUniform1fv(glGetUniformLocation(program, "lightSourcekl"), 160, kl);
	glUniform1fv(glGetUniformLocation(program, "lightSourcekq"), 160, kq);
}

void drawLamps()
{
	vec3 lampPos = bronze.pos;
	vec3 newLampPos = lampPos;

	MS_draw_mod(&bronze);
	MS_draw_mod(&steel);
	MS_draw_mod(&white);
	MS_draw_mod(&chain);

	newLampPos.x = newLampPos.x - 120;
	bronze.pos = newLampPos;
	steel.pos = newLampPos;
	white.pos = newLampPos;
	chain.pos = newLampPos;

	MS_update_modToWorld(&bronze);
	MS_update_modToWorld(&steel);
	MS_update_modToWorld(&white);
	MS_update_modToWorld(&chain);
	MS_draw_mod(&bronze);
	MS_draw_mod(&steel);
	MS_draw_mod(&white);
	MS_draw_mod(&chain);

	bronze.pos = lampPos;
	steel.pos = lampPos;
	white.pos = lampPos;
	chain.pos = lampPos;

	MS_update_modToWorld(&bronze);
	MS_update_modToWorld(&steel);
	MS_update_modToWorld(&white);
	MS_update_modToWorld(&chain);
}

void drawPanes()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	vec3 panePos = pane.pos;
	vec3 newpanePos = panePos;
	mat4 paneRot = pane.rot;
	mat4 newPaneRot = paneRot;

	MS_draw_mod(&pane);

	newpanePos.x = panePos.x+55;
	pane.pos = newpanePos;
	MS_update_modToWorld(&pane);
	MS_draw_mod(&pane);

	newpanePos.x = panePos.x+112;
	pane.pos = newpanePos;
	MS_update_modToWorld(&pane);
	MS_draw_mod(&pane);

	newpanePos.x = panePos.x+166.5;
	pane.pos = newpanePos;
	MS_update_modToWorld(&pane);
	MS_draw_mod(&pane);

	newpanePos.x = panePos.x-285;
	newpanePos.z = panePos.z+21.3;
	newPaneRot = Ry(M_PI);
	pane.pos = newpanePos;
	pane.rot = newPaneRot;
	MS_update_modToWorld(&pane);
	MS_draw_mod(&pane);

	newpanePos.x = panePos.x-285+53.5;
	newpanePos.z = panePos.z+21.3;
	newPaneRot = Ry(M_PI);
	pane.pos = newpanePos;
	pane.rot = newPaneRot;
	MS_update_modToWorld(&pane);
	MS_draw_mod(&pane);

	newpanePos.x = panePos.x-285+110;
	newpanePos.z = panePos.z+21.3;
	newPaneRot = Ry(M_PI);
	pane.pos = newpanePos;
	pane.rot = newPaneRot;
	MS_update_modToWorld(&pane);
	MS_draw_mod(&pane);

	newpanePos.x = panePos.x-285+165.5;
	newpanePos.z = panePos.z+21.3;
	newPaneRot = Ry(M_PI);
	pane.pos = newpanePos;
	pane.rot = newPaneRot;
	MS_update_modToWorld(&pane);
	MS_draw_mod(&pane);

	pane.pos = panePos;
	pane.rot = paneRot;
	
	MS_update_modToWorld(&pane);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void drawFrames()
{
	vec3 framePos = frame.pos;
	vec3 newframePos = framePos;

	MS_draw_mod(&frame);
	
	newframePos.x = framePos.x+55.5;
	frame.pos = newframePos;
	MS_update_modToWorld(&frame);
	MS_draw_mod(&frame);
	

	newframePos.x = framePos.x+112;
	frame.pos = newframePos;
	MS_update_modToWorld(&frame);
	MS_draw_mod(&frame);
	

	newframePos.x = framePos.x+166.5;
	frame.pos = newframePos;
	MS_update_modToWorld(&frame);
	MS_draw_mod(&frame);
	

	newframePos.x = framePos.x+5;
	newframePos.z = framePos.z-174;
	frame.pos = newframePos;
	MS_update_modToWorld(&frame);
	MS_draw_mod(&frame);
	

	newframePos.x = framePos.x+5+53.5;
	newframePos.z = framePos.z-174;
	frame.pos = newframePos;
	MS_update_modToWorld(&frame);
	MS_draw_mod(&frame);
	

	newframePos.x = framePos.x+5+110;
	newframePos.z = framePos.z-174;
	frame.pos = newframePos;
	MS_update_modToWorld(&frame);
	MS_draw_mod(&frame);
	

	newframePos.x = framePos.x+5+165.5;
	newframePos.z = framePos.z-174;
	frame.pos = newframePos;
	MS_update_modToWorld(&frame);
	MS_draw_mod(&frame);
	


	frame.pos = framePos;
	MS_update_modToWorld(&frame);
	
}

void drawPillars()
{
	vec3 oldColPos = pillarColumn.pos;
	vec3 oldFoundPos = pillarFoundation.pos;
	vec3 newColPos = oldColPos;
	vec3 newFoundPos = oldFoundPos;

	MS_draw_mod(&pillarFoundation);
	MS_draw_mod(&pillarColumn);
	
	newColPos.x = oldColPos.x+60;
	newFoundPos.x = oldFoundPos.x+60;
	pillarColumn.pos = newColPos;
	pillarFoundation.pos = newFoundPos;
	MS_update_modToWorld(&pillarFoundation);
	MS_update_modToWorld(&pillarColumn);
	MS_draw_mod(&pillarFoundation);
	MS_draw_mod(&pillarColumn);

	newColPos.x = oldColPos.x+120;
	newFoundPos.x = oldFoundPos.x+120;
	pillarColumn.pos = newColPos;
	pillarFoundation.pos = newFoundPos;
	MS_update_modToWorld(&pillarFoundation);
	MS_update_modToWorld(&pillarColumn);
	MS_draw_mod(&pillarFoundation);
	MS_draw_mod(&pillarColumn);

	newColPos.z = oldColPos.z+94;
	newFoundPos.z = oldFoundPos.z+94;
	newColPos.x = oldColPos.x;
	newFoundPos.x = oldFoundPos.x;
	pillarColumn.pos = newColPos;
	pillarFoundation.pos = newFoundPos;
	MS_update_modToWorld(&pillarFoundation);
	MS_update_modToWorld(&pillarColumn);
	MS_draw_mod(&pillarFoundation);
	MS_draw_mod(&pillarColumn);

	newColPos.z = oldColPos.z+94;
	newFoundPos.z = oldFoundPos.z+94;
	newColPos.x = oldColPos.x+60;
	newFoundPos.x = oldFoundPos.x+60;
	pillarColumn.pos = newColPos;
	pillarFoundation.pos = newFoundPos;
	MS_update_modToWorld(&pillarFoundation);
	MS_update_modToWorld(&pillarColumn);
	MS_draw_mod(&pillarFoundation);
	MS_draw_mod(&pillarColumn);

	newColPos.z = oldColPos.z+94;
	newFoundPos.z = oldFoundPos.z+94;
	newColPos.x = oldColPos.x+120;
	newFoundPos.x = oldFoundPos.x+120;
	pillarColumn.pos = newColPos;
	pillarFoundation.pos = newFoundPos;
	MS_update_modToWorld(&pillarFoundation);
	MS_update_modToWorld(&pillarColumn);
	MS_draw_mod(&pillarFoundation);
	MS_draw_mod(&pillarColumn);

	pillarColumn.pos = oldColPos;
	pillarFoundation.pos = oldFoundPos;
	MS_update_modToWorld(&pillarFoundation);
	MS_update_modToWorld(&pillarColumn);
}



void collisionHandling()
{
	for (int i = 0; i < nrCollisionObjects; i++)
	{
		int collDetect = collisionDetection(collisionList[i]);
		if (collDetect == 1)
		{
			resolveCollision(collisionList[i]);
		}
	}
}

void resolveAOBCollision()
{
	if (p.x < -182) {p.x = -182;}
	if (p.x > 73) {p.x = 73;}
	if (p.z < -69) {p.z = -69;}
	if (p.z > 90) {p.z = 90;}
}

int collisionDetection(MS *obj)
{
	if (p.z < obj->pos.z + obj->zsize && p.z > obj->pos.z - obj->zsize &&
		p.x < obj->pos.x + obj->xsize && p.x > obj->pos.x - obj->xsize)
		{
			return 1;
		}
	else {
		return 0;
	}
}

void resolveCollision(MS *obj)
{
	if(obj->xsize - fabs(obj->pos.x-p.x) < obj->zsize - fabs(obj->pos.z - p.z))
	{
		if (obj->pos.x - p.x < 0)
		{
			p.x = obj->pos.x + obj->xsize;
		} 
		else
		{
			p.x = obj->pos.x - obj->xsize;
		}
	}
	else
	{
		if (obj->pos.z - p.z < 0)
		{
			p.z = obj->pos.z + obj->zsize;
		} 
		else
		{
			p.z = obj->pos.z - obj->zsize;
		}
	}
}



void updateCameraPos(int x, int y)
{
	float mouseSense = 0.03;

	float dx = mousex-x;
	float dy = mousey-y;

	dx = mouseSense*dx;
	dy = mouseSense*dy;

	phi += -dx;
	theta += dy;

	if (theta > 89) {theta = 89;}
	if (theta < -89) {theta = -89;}

	vec3 looky;
	looky.x = cos(phi*3.14/180);
	looky.z = sin(phi*3.14/180);
	looky.y = sin(theta*3.14/180);
	l = Normalize(looky);
	glutWarpPointer(mousex,mousey);
	if (phi < 0)
	{
		phi = phi+360;
	}
	if (phi > 360)
	{
		phi = phi-360;
	}
}

void updatePosition()
{
	if (glutKeyIsDown('w'))
	{
		vec3 temp = {l.x, 0.0f, l.z};
		temp = Normalize(temp);
		p = VectorAdd(p, temp);
	}
	if (glutKeyIsDown('s'))
	{
		vec3 temp = {-l.x, 0.0f, -l.z};
		temp = Normalize(temp);
		p = VectorAdd(p, temp);
	}
	if (glutKeyIsDown('a'))
	{
		//p.x = p.x - 1;
		vec3 sideVec = CrossProduct(v,l);
		sideVec = Normalize(sideVec);
		p.x = p.x + sideVec.x;
		p.z = p.z + sideVec.z;
	}
	if (glutKeyIsDown('d'))
	{
		//p.x = p.x + 1;
		vec3 sideVec = CrossProduct(v,l);
		sideVec = Normalize(sideVec);
		p.x = p.x - sideVec.x;
		p.z = p.z - sideVec.z;

	}
	if (glutKeyIsDown('c'))
	{
		p.y = p.y - 1;
	}
	if (glutKeyIsDown('v'))
	{
		p.y = p.y + 1;
	}
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

