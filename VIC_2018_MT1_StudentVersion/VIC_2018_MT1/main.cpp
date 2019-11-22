// TODO: Lighting: Linear interpolation
// TODO: Barn + cows?

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include "Bitmap.h"
#include "Heightfield.h"

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include "Axes.h"

#include "CubeClass.h"

#include "Dome.h"
#include "2DText.h"
#include "grid.h"
#include "TaperedPrism.h"
#include "TriangularPrism.h"
#include "Flagpole.h"
#include "Colors.h"
#include "TexturedPolygons.h"
#include "Abductee.h"
#include "ParticleSystem.h"
#include "BoundingBoxes.h"

#define _USE_MATH_DEFINES
#include <math.h>

#pragma warning(disable : 4996) // Reduce unsafe warning messages

#define framesPerSecond 25
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame = false;

#define ESC	  27

#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99

static GLfloat	eyePoint[3] = { 21, 10, 57 };
static GLfloat	lookAtPoint[3];
static GLfloat	upVec[3];//
static GLfloat  fov; // field of view
static GLfloat	nearPlane;
static GLfloat	farPlane;

// Toggles between test mode (free movement of camera) and game mode (move ufo)
static bool debugMode = false;
// Toggles displaying the help overlay
static bool helpMode = true;

static int	WinWidth, WinHeight;
static int	AppMenu;

static GLfloat	spinAngle;
GLfloat spinIncrement=0.4f; // slow

// Used for the day / night cycle
// 9am = 180
// = 345
static GLfloat dayNightDegrees = 180;
static int dayNightTime = 6;
static GLfloat dayNightSpeed = 0.5;

static GLfloat icoSpinAngle;
static GLfloat icoSpinSpeed = 3;

static int ufoLight = 0;
static float ufoHeightOffset = 0;
static float ufoSpeed = 0.6;
static bool ufoDescending = false;
static bool ufoLanded = false;
static bool ufoAscending = false;
static int numAbductions = 0;

// Ufo Movement stuff.
static GLfloat ufoPos[] = { 43, 10, 85, 1 };
static GLfloat ufoDegrees = 0;
// Ufo Bounding Box
static GLfloat ufoBB[4][3];
static void setUfoBB() {
	// front left
	ufoBB[0][0] = ufoPos[0] - 2.25;
	ufoBB[0][1] = ufoPos[1] - 1;
	ufoBB[0][2] = ufoPos[2] + 2.25;
	// front right
	ufoBB[1][0] = ufoPos[0] + 2.25;
	ufoBB[1][1] = ufoPos[1] - 1;
	ufoBB[1][2] = ufoPos[2] + 2.25;
	// back right
	ufoBB[2][0] = ufoPos[0] + 2.25;
	ufoBB[2][1] = ufoPos[1] - 1;
	ufoBB[2][2] = ufoPos[2] - 2.25;
	// back left
	ufoBB[3][0] = ufoPos[0] - 2.25;
	ufoBB[3][1] = ufoPos[1] - 1;
	ufoBB[3][2] = ufoPos[2] - 2.25;
}
static GLfloat testUfoBB[4][3];
static void setTestUfoBB(GLfloat testX, GLfloat testZ) {
	testUfoBB[0][0] = testX - 2.25;
	testUfoBB[0][1] = ufoPos[1];
	testUfoBB[0][2] = testZ + 2.25;
	// front right
	testUfoBB[1][0] = testX + 2.25;
	testUfoBB[1][1] = ufoPos[1] - 1;
	testUfoBB[1][2] = testZ + 2.25;
	// back right
	testUfoBB[2][0] = testX + 2.25;
	testUfoBB[2][1] = ufoPos[1];
	testUfoBB[2][2] = testZ - 2.25;
	// back left
	testUfoBB[3][0] = testX - 2.25;
	testUfoBB[3][1] = ufoPos[1];
	testUfoBB[3][2] = testZ - 2.25;
}

// Declare an array to hold the abductees. They will need to be iterated over.
// Note that you need to declare the objects in a C++ array when you declare it, apparently
static Abductee abductees[9] = { 
	// NB: The version of Visual Studio running at UC requires casting all these to floats to avoid
	// compiler errors
	// Astronauts in spaceworld
	{(GLfloat)18, (GLfloat)3.1, (GLfloat)41, false, false, true, 0, 0, 0},
	{ (GLfloat)15, (GLfloat)3.1, (GLfloat)44, false, false, true, 1, 0, 0},
	{ (GLfloat)21, (GLfloat)3, (GLfloat)44, false, false, true, 2, 0, 0},
	// Power plant: construction workers?
	{ (GLfloat)35, (GLfloat)0.5, (GLfloat)57, true, false, false, 1, 0, 0},
	{ (GLfloat)36, (GLfloat)0.5, (GLfloat)56, true, false, false, 0, 0, 0},
	// Fireworks guys
	{ (GLfloat)40, (GLfloat)5.1, (GLfloat)8, true, false, false, 0, 0, 0},
	{ (GLfloat)42, (GLfloat)4.5, (GLfloat)8, true, false, false, 2, 0, 0},
	// Strange cows
	{ (GLfloat)40, (GLfloat)1.3, (GLfloat)40, false, true, false, 2, 0, 0},
	{ (GLfloat)43, (GLfloat)1.1, (GLfloat)41, false, true, false, 3, 0, 0}
};
static int numAbductees = 9;
#define ABDUCTION_SPEED 0.05;

static GLfloat satelliteRotationAngle;
static GLfloat satelliteRotationSpeed = 1;

GLfloat pulsationCounter;
bool pulsationIncreasing = true;
//static GLfloat pulsatingGreen[] = { (float)178 / 255, (float)255 / 255 , (wwwwfloat)35 / 255 };
static GLfloat pulsatingGreen[] = { (float)90 / 255, (float)135 / 255 , (float)0 / 255 };

// Lighting stuff
// Technically a direction, not a position
// The light in the direction of the vector FROM light0Pos TO the origin
//static GLfloat  light0Pos[4] = { -1, 0, 0, 0 };
static GLfloat  light0Pos[4] = { 0, 0, 1, 0 };
static GLfloat  light2Pos[4] = { 0, 0, -1, 0 };

//static GLfloat light2Pos[4] = { 20, 1, 10, 1 };
//static GLfloat light2Dir[3] = { -1, 0, 0 };

bool light0Enabled = true;
bool light1Enabled = true;
bool light2Enabled = true;

static bool drawNormalsAsLines = false;

static GLfloat	color3[3]; 

GLUquadricObj *quadric1;    
GLUquadricObj *quadric2;  
GLUquadricObj *quadric3;    
GLUquadricObj *quadric4;    

// Particle System
//ParticleSystem *ps1;
//ParticleSystem *ps2;
//ParticleSystem *ps3;
static ParticleSystem particleSystems[3] = { 
	{ 40, 6, 10, 1, 0, 0 },
	{ 41, 7, 10, 0, 1, 0 },
	{ 42, 6, 10, 0, 0, 1 } 
};
static GLfloat particleTimer = 0;
static bool particleSystemsRunning = true;
static bool particleSystemsStopping = false;
int numParticleSystems = 3;

Axes *axes; //
Grid *grid; // To help with push and scal etc

CubeClass *cube;
DomeClass *domeRoof;
DomeClass *domeSatellite;
DomeClass *domeUfoBubble;
DomeClass *domeUfoBody;
TaperedPrism *taperedPrism;
TaperedPrism *truncatedPyramid;
TaperedPrism *carFront;
TaperedPrism *carMiddle;
TriangularPrism *triangularPrism;
FlagPole *flagpole;

//int showGrid=true;
int showAxes=true;
int showBase1=true;
int showBase2=false;
int showWireFrame=false;
int showSpin=false;
int showChrisTrapani=false; // change center of spin 

int hiddenDepth=true;
int hiddenCull=true;
int clockWise=false; // start CCW
GLfloat angle = 0;
GLfloat angleVar = 0;

GLfloat centerX = 10;
GLfloat centerZ = 10;

Text2D *drawText2d;
char dispString[255] = "2=2D (also F1-help)  3=3D  5=2D+3D";

void resetMaterials() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, defaultShininess);
}

void setDefaultPolygonMode() {
	if (showWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}
}

void CheckGL()
{
	int  err;
	char msg[256];

	err = glGetError();
	if (err != GL_NO_ERROR) {
		sprintf(msg, "OpenGL error: %s", (GLubyte*)gluErrorString(err));
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

/****		Window events		****/

static void seteyePoint()
{
	glLoadIdentity();

	if (debugMode) {
		// User controls camera
		gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2],
		lookAtPoint[0], lookAtPoint[1], lookAtPoint[2],
		upVec[0], upVec[1], upVec[2]);
	}
	else {
		// Camera follows ufo
		gluLookAt(ufoPos[0] + (sin(-ufoDegrees * M_PI / 180) * 20),
			ufoPos[1] + 5,
			ufoPos[2] + (cos(-ufoDegrees * M_PI / 180) * 20),
			ufoPos[0], ufoPos[1], ufoPos[2],
			upVec[0], upVec[1], upVec[2]);


	}
}

static void setProjection3D()
{
  GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspect = (float)WinWidth / (float)WinHeight;
  gluPerspective(fov, aspect, nearPlane, farPlane);
  /* Back to normal */
  glMatrixMode(GL_MODELVIEW);
  seteyePoint();

  if (hiddenDepth)
  {
	// Now do z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthRange(nearPlane,farPlane);
	glClearDepth(farPlane);
  }
  else
  {
	glDisable(GL_DEPTH_TEST);
  }

  if (hiddenCull)
  {
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
	  if (clockWise) glFrontFace(GL_CW);
	  else glFrontFace(GL_CCW);
  }
  else
  {
	  glDisable(GL_CULL_FACE);
  }
}

static void setProjection2D()
{
  //GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //aspect = (float)WinWidth / (float)WinHeight;
  glOrtho(0, WinWidth, 0, WinHeight, -1.0f, 1.0f);
  /* Back to normal but identity*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
}

/***** Texture stuff *****/

CBitmap *cowSkin; 
CBitmap *tigerSkin;
CBitmap *earth;
CBitmap *sign;
CBitmap *ufpFlag;
CBitmap *controlPanel;
CBitmap *nasaLogo;
CBitmap *alienFlag;
CBitmap *moon;
CBitmap *grassTexture;
CBitmap *stoneTexture;
CBitmap *clouds;

// Not sure I actually need to declare the array size
GLuint textureHandles[12];

void readTextureFiles() {

	// Read in the image files
	cowSkin = new CBitmap("Textures\\cowPrint.bmp");
	tigerSkin = new CBitmap("Textures\\tigerSkin.bmp");
	earth = new CBitmap("Textures\\earth.bmp");
	sign = new CBitmap("Textures\\sign.bmp");
	ufpFlag = new CBitmap("Textures\\ufp.bmp");
	controlPanel = new CBitmap("Textures\\control_panel.bmp");
	nasaLogo = new CBitmap("Textures\\nasaLogo.bmp");
	alienFlag = new CBitmap("Textures\\alienFlag.bmp");
	moon = new CBitmap("Textures\\moon.bmp");
	grassTexture = new CBitmap("Textures\\grass.bmp");
	stoneTexture = new CBitmap("Textures\\stone.bmp");
	clouds = new CBitmap("Textures\\clouds.bmp");

	// Instruct OpenGL to give n texture handles to textureHandles
	glGenTextures(12, textureHandles);

	// Now we're dealing with the first texture
	glBindTexture(GL_TEXTURE_2D, textureHandles[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Set the basic values for that texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, cowSkin->GetWidth(), cowSkin->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, cowSkin->GetBits());

	// Now we're dealing with the second texture
	glBindTexture(GL_TEXTURE_2D, textureHandles[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Set basic values
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tigerSkin->GetWidth(), tigerSkin->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, tigerSkin->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, earth->GetWidth(), earth->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, earth->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, sign->GetWidth(), sign->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, sign->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ufpFlag->GetWidth(), ufpFlag->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, ufpFlag->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, controlPanel->GetWidth(), controlPanel->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, controlPanel->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, nasaLogo->GetWidth(), nasaLogo->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, nasaLogo->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, alienFlag->GetWidth(), alienFlag->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, alienFlag->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, moon->GetWidth(), moon->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, moon->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[9]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, grassTexture->GetWidth(), grassTexture->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, grassTexture->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[10]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, stoneTexture->GetWidth(), stoneTexture->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, stoneTexture->GetBits());

	glBindTexture(GL_TEXTURE_2D, textureHandles[11]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, clouds->GetWidth(), clouds->GetHeight(), 0, GL_RGBA,
		GL_UNSIGNED_BYTE, clouds->GetBits());

	// Set the texture environment (I think it's GL_MODULATE by default)
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Pass the relevant texture handles to the relevant abductees.
	for (int i = 0; i < numAbductees; i++) {
		if (abductees[i].isAstronaut) {
			abductees[i].texh1 = textureHandles[5];
			abductees[i].texh2 = textureHandles[6];
		}
		else if (abductees[i].isCow) {
			abductees[i].texh1 = textureHandles[0];
		}
	}
}

/***** Premade Methods *****/

void drawCylinder()
{	
	glPushMatrix();
	glTranslatef(0,0,-0.5);
	gluCylinder(quadric1,0.5,0.5,1,17,4);
	glPopMatrix();
}

void drawDisk(GLfloat innerRadius) // inner raduis 0 = full disk 0.5 = no disk or circle
{	
	glPushMatrix();
	gluDisk(quadric3,innerRadius,0.5,17,4);
	glPopMatrix();
}

void drawCone(GLfloat smallEndRadius) //  raduis 0 = Cone  0.5 = Cylinder
{	
	glPushMatrix();
	glTranslatef(0,0,-0.5);
	gluCylinder(quadric4,0.5,smallEndRadius,1,17,4);
	glPopMatrix();
}

void drawSphere()
{	
	glPushMatrix();
	gluQuadricTexture(quadric2, true);
	gluSphere(quadric2,0.5,13,13);
	glPopMatrix();
}

void drawDome()
{
	//int i;
	glPushMatrix();

	glPopMatrix();
}

void drawConector()
{
	glPushMatrix();

	glPopMatrix();
}

void drawDomeAndConector(GLfloat angle)
{
	glPushMatrix();

	glPopMatrix();
}
	
/***** Custom Methods *****/

void drawAntenna(float needleLength, float needleRadius, float topBitRadius,
	const GLfloat *topBitColor, const GLfloat *topBitAmbient,
	const GLfloat *needleColor, const GLfloat *needleAmbient) {
	glPushMatrix();
	// Needle
	glMaterialfv(GL_FRONT, GL_AMBIENT, needleAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, needleColor);
	glTranslatef(0, needleLength, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quadric1, needleRadius, needleRadius, needleLength, 10, 1);
	// Top bit
	glTranslatef(0, 0, -topBitRadius);
	//glMaterialfv(GL_FRONT, GL_EMISSION, topBitColor);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, topBitAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, topBitColor);
	gluSphere(quadric1, topBitRadius, 10, 10);
	glPopMatrix();

	//resetMaterials();
}

void drawSatellite(const GLfloat *supportColor, const GLfloat *supportColorAmbient, 
	const GLfloat *dishColor, const GLfloat *dishColorAmbient, const GLfloat *needleColor,
	const GLfloat *needleColorAmbient, const GLfloat *receiverColor, const GLfloat *receiverColorAmbient) {

	// Support
	glPushMatrix();
	glTranslatef(0, 0, -0.5);
	glRotatef(90, 1, 0, 0);
	glScalef(0.1, 0.3, 0.5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, supportColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, supportColor);
	taperedPrism->render();
	glPopMatrix();
	
	// Dish
	glPushMatrix();
	glTranslatef(0, 0.6, -0.2);
	glRotatef(60, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, dishColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dishColor);
	domeSatellite->renderNormalsIn();
	glTranslatef(0, 0.01, 0);
	domeSatellite->render();
	glPopMatrix();

	// Antenna
	glPushMatrix();
	glTranslatef(0, 0.6, -0.2);
	glRotatef(60, 1, 0, 0);
	drawAntenna(0.6, 0.02, 0.05, receiverColor, receiverColorAmbient, needleColor, needleColorAmbient);
	glPopMatrix();
}

void drawTree(const GLfloat *trunkColor, const GLfloat *leafColor) {

	// Mysterious icosahedron
	glPushMatrix();
	//glRotatef(icoSpinAngle, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	glColor3fv(black);
	glutSolidIcosahedron();
	glLineWidth(2);
	glColor3fv(leafColor);
	glPushMatrix();
	glScalef(1.05, 1.05, 1.05);
	glutWireIcosahedron();
	glPopMatrix();
	glLineWidth(1);
	glPopMatrix();
	// Support
	glPushMatrix();
	glTranslatef(0, 0, 0.7);
	glColor3fv(trunkColor);
	glScalef(1, 1, 6);
	glutSolidCube(0.1);
	glPopMatrix();
}

void drawDoor(const GLfloat* doorColor, const GLfloat* doorColorAmbient, 
	const GLfloat* handleColor, const GLfloat* handleColorAmbient) {
	glPushMatrix();
	
	glTranslatef(0.125, -0.60, 0.25);

	glPushMatrix();
	glScalef(0.25, 0.75, 0.5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, doorColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, doorColor);
	glutSolidCube(1);
	glPopMatrix();

	// TODO: Connect Handle to door?
	// Handle
	glPushMatrix();
	glTranslatef(0.2, 0, 0);
	glRotatef(90, 0, 1, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, handleColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, handleColor);
	glutSolidTorus(0.05, 0.2, 10, 10);
	// Spokes
	glRotatef(90, 0, 0, 1);
	glPushMatrix();
	glScalef(0.03, 0.3, 0.03);
	glutSolidCube(1);
	glPopMatrix();
	glRotatef(90, 0, 0, 1);
	glPushMatrix();
	glScalef(0.03, 0.3, 0.03);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	//resetMaterials();
}

void drawHouse(const GLfloat *windowColor, const GLfloat *windowColorAmbient, bool smallAntenna, 
	const GLfloat *smallAntennaColor, const GLfloat *smallAntennaAmbient, bool bigAntenna, 
	const GLfloat *bigAntennaColor, const GLfloat *bigAntennaAmbient) {
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);

	// House body
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(quadric1, 1, 1, 1, 10, 1);
	// First Floor Windows
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, windowColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, windowColor);
	glRotatef(90, 1, 0, 0);
	for (int i = 0; i < 2; i++) {
		glPushMatrix();
		glRotatef(-123 + (i * 72), 0, 1, 0);
		glTranslatef(0, 0.5, -1);
		glutSolidTorus(0.03, 0.15, 10, 10);
		glPopMatrix();
	}
	glPopMatrix();	// end first floor windows
	glPopMatrix();

	// House roof
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(180, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
	domeRoof->render();
	glPopMatrix();

	// Antennae
	glTranslatef(0.9, 0, 0);
	if (smallAntenna) {
		drawAntenna(1, 0.05, 0.1, smallAntennaColor, smallAntennaAmbient, white, defaultAmbient);
	}
	glTranslatef(0, 0, -0.3);
	if (bigAntenna) {
		drawAntenna(1.4, 0.05, 0.1, bigAntennaColor, bigAntennaAmbient, white, defaultAmbient);
	}

	// Door
	drawDoor(grey, defaultAmbient, red, redAmbient);

	glPopMatrix();

	//resetMaterials();
}

void drawBigHouse(const GLfloat *dividerColor, const GLfloat *dividerColorAmbient, 
	const GLfloat *firstFloorWindowColor, const GLfloat *firstFloorWindowAmbient, 
	const GLfloat *secondFloorWindowColor, const GLfloat *secondFloorWindowAmbient, 
	bool satellite, bool smallAntenna, const GLfloat *smallAntennaColor, 
	const GLfloat *smallAntennaAmbient, bool bigAntenna, const GLfloat *bigAntennaColor, 
	const GLfloat *bigAntennaAmbient) {

	glPushMatrix();
	glTranslatef(0, 1, 0);

	// First floor
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightgrey);
	gluCylinder(quadric1, 1, 1, 1, 10, 1);
	// First Floor Windows
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, firstFloorWindowAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, firstFloorWindowColor);
	glRotatef(90, 1, 0, 0);
	for (int i = 0; i < 2; i++) {
		glPushMatrix();
		glRotatef(-123 + (i * 72), 0, 1, 0);
		glTranslatef(0, 0.5, -1);
		glutSolidTorus(0.03, 0.15, 10, 10);
		glPopMatrix();
	}
	glPopMatrix();	// end first floor windows

	// Divider
	glTranslatef(0, 0, -0.1);
	glColor3fv(dividerColor);
	gluCylinder(quadric1, 1, 1, 0.1, 10, 1);

	// Second Floor
	glTranslatef(0, 0, -1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightgrey);
	gluCylinder(quadric1, 1, 1, 1, 10, 1);
	// Second Floor Windows
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, secondFloorWindowAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, secondFloorWindowColor);
	glRotatef(90, 1, 0, 0);
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glRotatef(-15 + (i * 72), 0, 1, 0);
		glTranslatef(0, 0.5, -1);
		glutSolidTorus(0.03, 0.15, 10, 10);
		glPopMatrix();
	}
	glPopMatrix();	// end second floor windows
	glPopMatrix();	// end second floor

	// Door
	glPushMatrix();
	glTranslatef(1, 0, -0.25);
	drawDoor(grey, defaultAmbient, red, redAmbient);
	glPopMatrix();

	// Flat Roof
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(0, 0, -1.1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
	glPushMatrix();
	glRotatef(180, 0, 1, 0); // Make sure the normal's facing outwards!
	gluDisk(quadric1, 0, 1, 10, 1);
	glPopMatrix();

	// Roof decorations
	glPushMatrix();
	// Low antenna
	glTranslatef(-0.6, 0.6, 0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	if (smallAntenna) {
		drawAntenna(1, 0.05, 0.1, smallAntennaColor, smallAntennaAmbient, darkgrey, defaultAmbient);
	}
	glPopMatrix();
	// High antenna
	glTranslatef(0, -0.2, 0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	if (bigAntenna) {
		drawAntenna(1.2, 0.05, 0.1, bigAntennaColor, bigAntennaAmbient, darkgrey, defaultAmbient);
	}
	glPopMatrix();
	// Mini satellite
	glTranslatef(0.6, -0.4, 0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glRotatef(135, 0, 1, 0);
	glTranslatef(0, 0, 0.8);
	if (satellite) {
		glScalef(0.5, 0.5, 0.5);
		drawSatellite(darkgrey, defaultAmbient, white, defaultAmbient, grey, defaultAmbient, blue,
			blueAmbient);
	}	
	glPopMatrix();

	glPopMatrix();	// End roof decorations
	glPopMatrix(); //  End roof

	glPopMatrix();

	//resetMaterials();
}

void drawUfo() {

	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	// Cockpit
	glPushMatrix();
	glTranslatef(0, -0.25, 0);
	glScalef(1.5, 1.5, 1.5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, skyBlue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, skyBlue);
	domeUfoBubble->render();
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	glPopMatrix();

	// Upper body
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glScalef(5, 5, 5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
	domeUfoBody->render();
	glPopMatrix();

	// Bottom
	glPushMatrix();
	glTranslatef(0, 0.9, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(2.5, 2.5, 2.5);
	gluDisk(quadric1, 0, 1, 20, 1);
	glPopMatrix();

	// Cone of light with alpha blending... hopefully
	glPushMatrix();
	glTranslatef(0, 1.5, 0);
	glRotatef(-90, 1, 0, 0);
	// Draw the transparent cone
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColor4f(0, 1, 1, 0.2);
	glCullFace(GL_FRONT);
	gluCylinder(quadric1, 1, 2, 9, 10, 1);
	glCullFace(GL_BACK);
	gluCylinder(quadric1, 1, 2, 9, 10, 1);
	// Reset stuff
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPopMatrix();

	// Decorative dots
	for (int i = 0; i < 15; i++) {
		glPushMatrix();
		glRotatef(24 * i, 0, 1, 0);
		glTranslatef(2, 0.67, 0);
		glRotatef(90, 1, 0, 0);
		glRotatef(25, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);

		if ((ufoLight / 2) % 15 == (i % 15)) {
			glMaterialfv(GL_FRONT, GL_EMISSION, brightGreen);
			glutSolidTorus(0.3, 1, 10, 10);
		}
		else if ((ufoLight / 2) % 15 == (i % 15) + 1) {
			glMaterialfv(GL_FRONT, GL_EMISSION, medGreen);
			glutSolidTorus(0.3, 1, 10, 10);
		}
		else if ((ufoLight / 2) % 15 == (i % 15) + 2) {
			glColor3fv(dullGreen);
			glMaterialfv(GL_FRONT, GL_EMISSION, dullGreen);
			glutSolidTorus(0.3, 1, 10, 10);
		}
		else {
			glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, greyGreen);
			glutSolidTorus(0.3, 1, 10, 10);
		}
		glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
		glPopMatrix();
	}
	glPopMatrix();

	//resetMaterials();
}

void drawLandingPad() {

	// Landing pad
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glScalef(6, 0.2, 6);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glutSolidCube(1);
	glPopMatrix();
	
	// Guide lights with lighting routine
	glPushMatrix();
	glTranslatef(-2.5, 0.7, -2.5);
	switch (ufoLight) {
	case 0:
	case 75:
		for (int i = 0; i < 4; i++) {
			glTranslatef(5, 0, 0);
			glRotatef(-90, 0, 1, 0);
			if (i % 2 == 0) {
				glMaterialfv(GL_FRONT, GL_EMISSION, white);
			}
			else {
				glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
			}
			glutSolidCube(0.2);
			glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
		}
		break;
	case 38:
	case 113:
		for (int i = 0; i < 4; i++) {
			glTranslatef(5, 0, 0);
			glRotatef(-90, 0, 1, 0);
			if (i % 2 == 1) {
				glMaterialfv(GL_FRONT, GL_EMISSION, red);
			}
			else {
				glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
			}
			glutSolidCube(0.2);
			glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
		}
		break;
	default:
		glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
		for (int i = 0; i < 4; i++) {
			glTranslatef(5, 0, 0);
			glRotatef(-90, 0, 1, 0);
			glutSolidCube(0.2);
		}
		break;
	}
	glPushMatrix();
	glPopMatrix();
	glPopMatrix();
	// TODO: Landing circle?

	//resetMaterials();
}

void drawBigSatelliteDish() {
	// Draw solid dish
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glScalef(2, 2, 2);
	glRotatef(45, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
	
	domeSatellite->renderNormalsIn();
	glPushMatrix();
	glTranslatef(0, -0.01, 0);
	glScalef(1.01, 1.01, 1.01);
	domeSatellite->render();
	glPopMatrix();

	// Draw dish scaffolding
	glDisable(GL_CULL_FACE);
	glPushMatrix();
	glScalef(1.1, 1.1, 1.1);
	glTranslatef(0, -0.05, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verydarkgrey);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2);
	domeSatellite->render();
	setDefaultPolygonMode();
	glLineWidth(1);
	glPopMatrix();
	glEnable(GL_CULL_FACE);

	// Draw behind-dish cylinder
	glPushMatrix();
	glPushMatrix();
	glScalef(0.125, 0.5, 0.125);
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
	gluCylinder(quadric1, 1, 1, 1, 10, 1);
	glPopMatrix();

	// Draw one more cylinder bit
	glTranslatef(0, -0.5, 0);
	glPushMatrix();
	glScalef(0.1, 0.1, 0.1);
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verydarkgrey);
	gluCylinder(quadric1, 1, 1, 1, 10, 1);
	glPopMatrix();

	glPopMatrix();

	// Draw dish sticks
	glPushMatrix();
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glRotatef(120 * i, 0, 1, 0);
		glTranslatef(0, 0.65, -0.4);
		glRotatef(45, 1, 0, 0);
		glScalef(0.025, 0.8, 0.025);
		glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		glutSolidCube(1);
		glPopMatrix();
	}
	glPopMatrix();

	// Draw signal receiver
	glPushMatrix();
	glTranslatef(0, 0.95, 0);
	glScalef(0.1, 0.05, 0.1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightgrey);
	gluSphere(quadric1, 1, 10, 10);
	glPopMatrix();

	glPopMatrix();

	//resetMaterials();
}

void drawBigSatellite(bool rotateSatellite) {

	// Upper support and dish
	glPushMatrix();
	glTranslatef(0, 1.5, -0.3);
	// rotate must be called here!
	if (rotateSatellite) {
		glRotatef(satelliteRotationAngle, 0, 1, 0);
	}
	glPushMatrix();
	glScalef(0.25, 0.8, 0.25);
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
	gluCylinder(quadric1, 1, 1, 1, 10, 1);
	glPopMatrix();

	glTranslatef(0, -1.5, 0.3);
	drawBigSatelliteDish();
	glPopMatrix();

	// TODO: Make a lower support from a truncated pyramid?
}

void drawTreeCluster() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	drawTree(brown, green);
	glTranslatef(0.7, -0.8, 0);
	drawTree(brown, green);
	glTranslatef(0.7, 0.8, 0);
	drawTree(brown, green);
	glPopMatrix();
}

void drawAtomSymbol() {
	glMaterialfv(GL_FRONT, GL_EMISSION, limeGreen);
	gluDisk(quadric1, 0, 0.1, 10, 1);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glRotatef(120 * i, 0, 0, 1);
		glScalef(1, 0.3, 1);
		gluDisk(quadric1, 0.8, 1, 15, 1);
		glPopMatrix();
	}
	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);

}

void drawCoolingTower() {

	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, darkgrey);
	gluCylinder(quadric1, 1.4, 1, 1, 10, 1);
	glTranslatef(0, 0, 1);
	gluCylinder(quadric1, 1, 0.8, 1, 10, 1);
	glTranslatef(0, 0, 1);
	gluCylinder(quadric1, 0.8, 0.8, 0.75, 10, 1);
	glTranslatef(0, 0, 0.75);
	glDisable(GL_CULL_FACE);
	gluCylinder(quadric1, 0.8, 0.85, 0.2, 10, 1);
	glEnable(GL_CULL_FACE);
	// Draw a 'lid'
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verydarkgrey);
	gluDisk(quadric1, 0, 0.8, 10, 1);

	// Draw atom symbol
	glPushMatrix();
	glTranslatef(0.79, 0, -0.3);
	glRotatef(90, 0, 1, 0);
	glScalef(0.25, 0.25, 0.25);
	drawAtomSymbol();
	glPopMatrix();

	glPopMatrix();

	//resetMaterials();
}

void drawSquareWindow(const GLfloat* borderColor, const GLfloat *borderColorAmbient, 
	const GLfloat* paneColor, const GLfloat *paneColorAmbient, GLfloat windowThickness) {

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, borderColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, borderColor);
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glScalef(windowThickness, windowThickness, 1);
		glutSolidCube(1);
		glPopMatrix();
		glTranslatef(0, -0.5, 0.5);
		glRotatef(90, 1, 0, 0);
	}
	glTranslatef(0, -0.5, 0);
	glScalef(0.01, 1, 1);

	glMaterialfv(GL_FRONT, GL_EMISSION, paneColor);

	//glMaterialfv(GL_FRONT, GL_AMBIENT, paneColorAmbient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, paneColor);
	glutSolidCube(1);

	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);

	glPopMatrix();

	//resetMaterials();
}

void drawPulsatingAntennaArray() {
	drawAntenna(1, 0.05, 0.2, pulsatingGreen, pulsatingGreen, verydarkgrey, defaultAmbient);
	glTranslatef(0, 0, -0.5);
	drawAntenna(1.4, 0.05, 0.2, pulsatingGreen, pulsatingGreen, verydarkgrey, defaultAmbient);
	glTranslatef(0, 0, -0.5);
	drawAntenna(1, 0.05, 0.2, pulsatingGreen, pulsatingGreen, verydarkgrey, defaultAmbient);
}

void drawPlantControlBuilding() {
	
	glPushMatrix();
	glTranslatef(0, 0.625, 1);

	glPushMatrix();
	glScalef(2.5, 1.25, 5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(1.3, 0.35, -0.3);
	drawDoor(darkgrey, defaultAmbient, red, redAmbient);

	// RHS windows
	glPushMatrix();
	glTranslatef(0, -0.2, -0.7);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glTranslatef(0, 0, i * -0.3);
		glScalef(1, 0.3, 0.3);
		drawSquareWindow(black, black, lightgrey, defaultAmbient, 0.05);
		glPopMatrix();
	}
	glPopMatrix();

	// LHS windows
	glPushMatrix();
	glTranslatef(0, -0.2, 1.8);
	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glTranslatef(0, 0, i * -0.3);
		glScalef(1, 0.3, 0.3);
		drawSquareWindow(black, black, lightgrey, defaultAmbient, 0.05);
		glPopMatrix();
	}
	glPopMatrix();

	// Roof decorations
	glPushMatrix();
	glTranslatef(-2, 0.275, -0.5);
	
	drawPulsatingAntennaArray();
	
	glPushMatrix();
	glTranslatef(0, 0, 4);
	drawPulsatingAntennaArray();
	glPopMatrix();

	// Small chimney
	glTranslatef(1.5, 0, -0.5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verydarkgrey);
	glPushMatrix();
	glTranslatef(0, 0.2, 0);
	glScalef(0.1, 0.4, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(-1, -1.5, 6.5);
	glPushMatrix();
	glRotatef(60, 0, 1, 0);
	glScalef(0.3, 0.3, 0.3);
	drawBigSatellite(false);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void drawPowerPlant() {

	glPushMatrix();

	glPushMatrix();
	glScalef(1.5, 1.5, 1.5);
	drawCoolingTower();
	glPopMatrix();

	glTranslatef(0, 0, 5);

	glPushMatrix();
	glScalef(1.5, 1.5, 1.5);
	drawCoolingTower();
	glPopMatrix();

	glTranslatef(4, 0, -3);
	
	drawPlantControlBuilding();

	glPopMatrix();


}

void drawHuman() {

	glPushMatrix();

	// Single colour for now
	glMaterialfv(GL_FRONT, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);

	// Body
	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	glScalef(0.5, 0.5, 0.3);
	glutSolidCube(1);
	glPopMatrix();

	// Head
	glPushMatrix();
	glTranslatef(0, 0.8, 0);
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	// Arms
	glPushMatrix();
	glTranslatef(0.3, 0.3, 0);
	glScalef(0.1, 0.5, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.3, 0.3, 0);
	glScalef(0.1, 0.5, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.8, 0.9, 0.8);
	// Lower Body
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glScalef(0.5, 0.2, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	// Legs
	glPushMatrix();
	glTranslatef(0.15, -0.35, 0);
	glScalef(0.2, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.15, -0.35, 0);
	glScalef(0.2, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

}

void drawCylinderWithLid() {

	glPushMatrix();
	gluCylinder(quadric1, 1, 1, 1, 10, 1);
	//glRotatef(90, 1, 0, 0);
	gluDisk(quadric1, 0, 1, 10, 1);
	glPopMatrix();

}

void drawEarth() {

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureHandles[2]);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, textureHandles[1]);

	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultDiffuse);
	gluQuadricTexture(quadric2, true);
	gluSphere(quadric2, 1, 10, 10);

	glDisable(GL_TEXTURE_2D);
}

// TODO: Nostrils? Ears? Cowbell?
void drawCow() {

	//Body
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glScalef(1.5, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

	// Legs
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);

	glTranslatef(-0.6, -0.6, 0.35);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(1.2, 0, 0);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(0, 0, -0.7);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(-1.2, 0, 0);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	// Tail
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glTranslatef(-0.65, 0.4, 0);
	glRotatef(60, 0, 0, 1);
	glPushMatrix();
	glScalef(1, 0.1, 0.1);
	glRotatef(-90, 0, 1, 0);
	glutSolidCone(1, 1, 10, 1);
	glPopMatrix();
	glPopMatrix();

	// Head
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glTranslatef(1, 0.5, 0);
	glPushMatrix();
	glScalef(0.2, 0.7, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	// Horns
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);

	glTranslatef(0, 0.3, 0.2);
	
	glPushMatrix();
	glRotatef(-45, 1, 0, 0);
	glPushMatrix();
	glScalef(0.05, 0.05, 0.25);
	glutSolidCone(1, 1, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glTranslatef(0, 0, -0.4);

	glPushMatrix();
	glRotatef(-130, 1, 0, 0);
	glPushMatrix();
	glScalef(0.05, 0.05, 0.25);
	glutSolidCone(1, 1, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	// Nose
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, pinkAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pink);
	glTranslatef(0.2, -0.15, 0);
	glScalef(0.1, 0.15, 0.25);
	glRotatef(-90, 0, 1, 0);
	drawCylinderWithLid();
	glPopMatrix();

	// Eyes
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, blueAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glTranslatef(0.125, 0.225, 0.125);
	glPushMatrix();
	glScalef(0.025, 0.05, 0.05);
	glRotatef(-90, 0, 1, 0);
	drawCylinderWithLid();
	glPopMatrix();
	glTranslatef(0, 0, -0.25);
	glPushMatrix();
	glScalef(0.025, 0.05, 0.05);
	glRotatef(-90, 0, 1, 0);
	drawCylinderWithLid();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix(); // end head

}

// NB: This was initially designed with the assumption it'd be behind a sign...
void drawHollowTriangle() {
	glPushMatrix();
	glTranslatef(0.05, 0, -0.3);
	glScalef(0.05, 0.05, 1.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, 0, -0.3);
	glRotatef(-25, 0, 1, 0);
	glScalef(0.05, 0.05, 1.4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, 0, -0.925);
	glScalef(0.6, 0.05, 0.05);
	glutSolidCube(1);
	glPopMatrix();
}

void drawSpaceWorldSign() {

	glMaterialfv(GL_FRONT, GL_EMISSION, verydarkgrey);
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightgrey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);

	// Billboard
	glPushMatrix();
	glScalef(0.05, 3, 1);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, textureHandles[3]);
	drawCubeOneSideTextured(1);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);

	// Supports
	glMaterialfv(GL_FRONT, GL_AMBIENT, brownAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, brown);
	glTranslatef(0, -2, 0);
	for (int i = 0; i < 3; i++) {
		glTranslatef(0, 1, 0);
		drawHollowTriangle();
	}
	

}

void drawUpperCar(const GLfloat *carColor, const GLfloat *carColorAmbient) {

	glPushMatrix();

	glTranslatef(0.1, 0.35, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(0.5, 0.3, 0.25);
	glPushMatrix();
	glPushMatrix();
	glScalef(1, 1.65, 1);
	carFront->render(); // front section

	// Hopefully this will look like a windscreen...
	glMaterialfv(GL_FRONT, GL_AMBIENT, verydarkgrey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verydarkgrey);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 50);
	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(0, -0.175, 0);
	carFront->render();
	glPopMatrix();

	// And hopefully this will look like a front side window - one side
	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(0.25, 0, 0);
	carFront->render();
	glPopMatrix();

	// front side window - other side
	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(-0.25, 0, 0);
	carFront->render();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, carColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, carColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMateriali(GL_FRONT, GL_SHININESS, 0);

	glPopMatrix();
	glTranslatef(0, 2.65, 0);
	glRotatef(180, 0, 0, 1);
	carFront->render(); // Rear window

	// Rear window stuff goes here...
	glMaterialfv(GL_FRONT, GL_AMBIENT, verydarkgrey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verydarkgrey);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMateriali(GL_FRONT, GL_SHININESS, 50);
	// Rear window - one side
	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(0.25, 0.25, 0);
	carFront->render();
	glPopMatrix();
	// Rear window - other side
	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(-0.25, 0.25, 0);
	carFront->render();
	glPopMatrix();
	// Rear window
	glPushMatrix();
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(0, -0.25, 0);
	carFront->render();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glMateriali(GL_FRONT, GL_SHININESS, 0);

	glPopMatrix();

	glPopMatrix();

}

void drawHeadlight() {
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(0.125, 0.125, 0.125);
	domeSatellite->render();
	glPopMatrix();
}

void drawTaillight() {
	glPushMatrix();
	glutSolidTorus(0.2, 1, 10, 10);
	glPopMatrix();
}

void drawCar(const GLfloat *carColor, const GLfloat *carColorAmbient) {

	// Base
	glMaterialfv(GL_FRONT, GL_AMBIENT, carColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, carColor);
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glScalef(2, 0.3, 1);
	glutSolidCube(1);
	glPopMatrix();

	// Door handle
	glMaterialfv(GL_FRONT, GL_AMBIENT, verydarkgrey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, verydarkgrey);
	glPushMatrix();
	glTranslatef(-0.1, 0.1, 0.5);
	glScalef(0.1, 0.025, 0.025);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1, 0.1, 0.5);
	glScalef(0.1, 0.025, 0.025);
	glutSolidCube(1);
	glPopMatrix();

	// Headlights
	glMaterialfv(GL_FRONT, GL_EMISSION, white);
	glPushMatrix();
	glTranslatef(1.05, -0.03, 0.35);
	glRotatef(90, 0, 1, 0);
	drawHeadlight();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.05, -0.03, -0.35);
	glRotatef(90, 0, 1, 0);
	drawHeadlight();
	glPopMatrix();

	// Taillights
	glMaterialfv(GL_FRONT, GL_EMISSION, red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glPushMatrix();
	glTranslatef(-1.025, -0.03, 0.35);
	glRotatef(90, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	drawTaillight();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.025, -0.03, -0.35);
	glRotatef(90, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	drawTaillight();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);


	// Top part
	glMaterialfv(GL_FRONT, GL_AMBIENT, carColorAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, carColor);
	drawUpperCar(carColor, carColorAmbient);

}

void drawFirework() {

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
	gluCylinder(quadric1, 0.5, 0.5, 5, 10, 1);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(quadric1, 0.6, 0, 1, 10, 1);
	glPopMatrix();
}

void drawSkyBox() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	// Sky
	glColor3f(skyColor[0], skyColor[1], skyColor[2]);
	//gluSphere(quadric1, 65, 10, 10);

	//glutSolidCube(200);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureHandles[11]);
	drawSkyBoxWithClouds(200);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_CULL_FACE);

	glPushMatrix();
	glRotatef(-dayNightDegrees, 1, 0, 0);

	// Sun
	glPushMatrix();
	glColor3fv(orange);	
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0, 85);
	gluSphere(quadric1, 5, 20, 20);
	glPopMatrix();
	// Moon
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureHandles[8]);
	gluQuadricTexture(quadric2, true);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPushMatrix();
	glColor3fv(white);
	glRotatef(225, 1, 0, 0);
	glTranslatef(0, 0, 85);
	gluSphere(quadric2, 5, 20, 20);
	glPopMatrix();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

/***** Test draw method *****/

void drawTest() {

	glPushMatrix();
	
	glTranslatef(0, 1, 0);

	drawHeightField(textureHandles[9], textureHandles[10]);

	glPopMatrix();

}

/***** Draw the moonbase *****/

void drawSpaceWorld()
{
	// Flag
	glPushMatrix();
	glTranslatef(10, 0.1, 10);
	glRotatef(30, 0, 1, 0);
	flagpole->render();
	//resetMaterials();
	glPopMatrix();

	// Landing Pad
	glPushMatrix();
	glTranslatef(3.5, -0.45, 3.5);
	drawLandingPad();
	glPopMatrix();

	// Houses along far edge of scene
	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	glTranslatef(2, 0, -9);
	drawHouse(orange, orangeAmbient, true, red, redAmbient, true, green, greenAmbient);
	glTranslatef(0, 0, -2.5);
	drawBigHouse(orange, orangeAmbient, skyBlue, skyBlueAmbient, skyBlue, skyBlueAmbient, true, true, green, greenAmbient, true, orange, orangeAmbient);
	glTranslatef(0, 0.2, -2.5);
	drawHouse(orange, orangeAmbient, true, red, redAmbient, true, green, greenAmbient);
	glTranslatef(0, 0, -2.5);
	drawHouse(orange, orangeAmbient, true, red, redAmbient, true, green, greenAmbient);
	// Inner row
	glTranslatef(6, 0, 0);
	glRotatef(180, 0, 1, 0);
	drawBigHouse(orange, orangeAmbient, skyBlue, skyBlueAmbient, skyBlue, skyBlueAmbient, true, true, green, greenAmbient, true, orange, orangeAmbient);
	glTranslatef(0, 0, -2.5);
	drawHouse(orange, orangeAmbient, true, cyan, cyanAmbient, false, cyan, cyanAmbient);
	glPopMatrix();

	// Big Satellite 
	glPushMatrix();
	glTranslatef(19, -0.5, 22);
	drawBigSatellite(true);
	glPopMatrix();

	// Sign
	glPushMatrix();
	glTranslatef(5, 1, 15);
	glRotatef(-90, 1, 0, 0);
	glRotatef(120, 0, 0, 1);
	glScalef(2, 2, 2);
	drawSpaceWorldSign();
	glPopMatrix();
}

void drawBoundingBoxes() {
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3fv(red);
	glPointSize(3);
	glBegin(GL_POINTS);
	// Flagpole
	for (int i = 0; i < 8; i++) {
		glVertex3fv(flagPoleBB[i]);
	}
	// Ufo
	for (int i = 0; i < 4; i++) {
		glVertex3fv(ufoBB[i]);
	}
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPointSize(1);
}

static void drawWorld ()
{
	setDefaultPolygonMode();
	glPushMatrix();

	if (showSpin)
	{
		glPushMatrix();
		glTranslatef(centerX, 0, centerZ); // move origin
		glRotatef(spinAngle, 0, 1, 0);
		glTranslatef(-centerX, 0, -centerZ); // move origin back
	}

	if (showBase1)
	{
		// Declare the directional lights
		glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sunLight);

		glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, moonLight);

		// Draw heightfield
		drawHeightField(textureHandles[9], textureHandles[10]);
		// Draw the normals as points, for now
		if (drawNormalsAsLines) {
			drawHeightFieldNormalsAsLines();
		}

		// Draw the sea
		glMaterialfv(GL_FRONT, GL_AMBIENT, seaBlueAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, seaBlue);
		glPushMatrix();
		glTranslatef(50, 0, 50);
		glScalef(100, 0.2, 100);
		glutSolidCube(1);
		glPopMatrix();

		// SpaceWorld
		glPushMatrix();
		glTranslatef(5, 4, 25);
		drawSpaceWorld();
		glPopMatrix();

		// Test: Draw Bounding Boxes
		//drawBoundingBoxes();

		// Power Plant
		glPushMatrix();
		glTranslatef(30, 1, 55);
		glRotatef(-30, 0, 1, 0);
		drawPowerPlant();
		glPopMatrix();

		// Run multiple particle systems. Hopefully
		for (int i = 0; i < numParticleSystems; i++) {
			if (particleSystems[i].isRunningRocket) {
				glPushMatrix();
				glTranslatef(particleSystems[i].psPosition[0], 
					particleSystems[i].psPosition[1] + 0.5, 
					particleSystems[i].psPosition[2]);
				glPushMatrix();
				glScalef(0.1, 0.1, 0.1);
				drawFirework();
				glPopMatrix();
				glPopMatrix();
				particleSystems[i].psPosition[1] += 0.1;
				particleSystems[i].renderRocket();
			}
			else if (particleSystems[i].isRunningFireworks) {
				particleSystems[i].renderFireworks();
			}
		}

		// Position the abductees
		for (int i = 0; i < numAbductees; i++) {
			if (!abductees[i].abducted) {
				abductees[i].render();
			}
		}

		// Draw the ufo last, so transparency works...
		// Refactor LIGHT1....
		if (!debugMode) {
			glPushMatrix();
			glTranslatef(ufoPos[0], ufoPos[1], ufoPos[2]);
			drawUfo();
			// Draw the skysphere relative to the ufo
			glPushMatrix();
			//glRotatef(-dayNightDegrees, 1, 0, 0);
			drawSkyBox();
			glPopMatrix();
			glPopMatrix();
			glEnable(GL_LIGHT1);
			glLightfv(GL_LIGHT1, GL_POSITION, ufoPos);
		}
		else {
			glPushMatrix();
			glTranslatef(eyePoint[0], eyePoint[1], eyePoint[2]);
			glPushMatrix();
			//glRotatef(-dayNightDegrees, 1, 0, 0);
			drawSkyBox();
			glPopMatrix();
			glPopMatrix();
		}
	}

	 
	  if (showBase2)
	  {
		  //drawBase2();
	  }
	  if (showSpin) glPopMatrix();
  
  glPopMatrix();

}



void drawHelpText()
{
	glDisable(GL_LIGHTING);
	glColor3f(1,1,0);
	
	int yPos = glutGet(GLUT_WINDOW_HEIGHT) - 25;

	drawText2d->drawstr(10, yPos, "***** ALIEN ABDUCTION GAME *****");

	if (debugMode) {
		drawText2d->drawstr(10, yPos - 60, "%s %8.2f,%8.2f,%8.2f, ", "EyePoint", eyePoint[0], eyePoint[1], eyePoint[2]);
	}
	else {
		drawText2d->drawstr(10, yPos - 60, "%s %8.2f,%8.2f,%8.2f, ", "UfoPos", ufoPos[0], ufoPos[1], ufoPos[2]);
	}

	drawText2d->drawstr(10, yPos - 120, "H:      Toggle Help");
	drawText2d->drawstr(10, yPos - 150, "IJKL:   Move UFO");
	drawText2d->drawstr(10, yPos - 180, "U/O:    Rotate UFO left / right");
	drawText2d->drawstr(10, yPos - 210, "P:      Abduct");
	drawText2d->drawstr(10, yPos - 240, "-/+:    Decrease / increase speed of day/night cycle");

	drawText2d->drawstr(10, yPos - 300, "Press F1 to toggle debug mode. Debug commands:");

	drawText2d->drawstr(10, yPos - 360, "Arrow keys and PageUp/PageDown move the camera");
	drawText2d->drawstr(10, yPos - 390, "W:      Toggle Wireframe");
	drawText2d->drawstr(10, yPos - 420, "D:      Toggle Depth Buffering");
	drawText2d->drawstr(10, yPos - 450, "C:      Toggle Backface Culling");
	drawText2d->drawstr(10, yPos - 480, "@:      Toggle CW / CCW Culling");
	drawText2d->drawstr(10, yPos - 510, "N:      Toggle Display Normals as Lines");
	//drawText2d->drawstr(10, yPos - 510, "Space:  Toggle Spin");
	//drawText2d->drawstr(10, yPos - 540, "/:      Adjust Spin Speed");

	glEnable(GL_LIGHTING);
  }

void draw2DOverlay() {

	int xPos = glutGet(GLUT_WINDOW_WIDTH) - 120;
	int yPos = glutGet(GLUT_WINDOW_HEIGHT) - 20;

	glDisable(GL_LIGHTING);
	glColor3fv(red);
	drawText2d->drawstr(xPos, yPos, "Time: %d:00", dayNightTime);
	drawText2d->drawstr(xPos, yPos - 30, "Abductions: %d", numAbductions);
	glEnable(GL_LIGHTING);
}

static void display()
{
  if (hiddenDepth) {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
  else {glClear(GL_COLOR_BUFFER_BIT);}

  setProjection3D();
  seteyePoint();
  drawWorld();

  // Draw the score and the time
  setProjection2D();
  draw2DOverlay();
  if (helpMode) {
	  drawHelpText();
  }

  /* Check everything OK and update screen */
  CheckGL();
  glutSwapBuffers();
}

static void resize (int width, int height)
{
  /* Save for event handlers */
  WinWidth  = width;
  WinHeight = height;
  
  /* Reset view in window. */
  glViewport(0, 0, WinWidth, WinHeight);
}


/****		User events		****/


static void menuChoice (int item)
{
  switch (item) {
    case cmdRed:
        color3[0]=1; // R
        color3[1]=0; // G
        color3[2]=0; // B
      break;
    case cmdGreen:
        color3[0]=0; // R
        color3[1]=1; // G
        color3[2]=0; // B
      break;
    case cmdExit:
      exit(0);
      break;
    default:
      break;
  }
}

// This is going to be very rough
static bool anyTerrainCollisions(GLfloat testX, GLfloat testZ) {
	// Roughly snap testX and testZ to coordinates in the heightfield
	int heightFieldZ = (int)(testZ * 4) + 24;
	if (heightFieldZ > 399) {
		heightFieldZ = 399;
	}
	if (heightFieldZ < 0) {
		heightFieldZ = 0;
	}
	int heightFieldX = (int)(testX * 4) + 47;
	if (heightFieldX > 399) {
		heightFieldX = 399;
	}
	if (heightFieldX < 0) {
		heightFieldX = 0;
	}

	// Find the indicated height, scale it, and compare
	// For some reason the heightfield and the rendered terrain don't seem to be lining 
	// up properly. I will try to look at the actual triangle vertex positions
	//GLfloat averageHeight = triangles[heightFieldZ * 798 + heightFieldX * 2][0][1];
	//printf("Height at [%d][%d]: %.2f\n", heightFieldX, heightFieldZ, averageHeight);

	GLfloat heightFieldY = heightFieldArray[heightFieldX][heightFieldZ];	
	//printf("Height of terrain at [%d][%d]: %.2f\n", heightFieldX, heightFieldZ,
	//	heightFieldY);
	heightFieldY *= 0.05;

	int hfX = heightFieldX < 349 ? heightFieldX + 50 : heightFieldX;
	int hfZ = heightFieldZ < 369 ? heightFieldZ + 30 : heightFieldZ;
	GLfloat hfY = heightFieldArray[hfX][hfZ];
	hfY *= 0.05;
	//printf("Height of terrain at [%d][%d]: %.2f\n", hfX, hfZ,
	//	hfY);
	//printf("Ufo Pos: %.2f %.2f %.2f\n", ufoPos[0], ufoPos[1], ufoPos[2]);

    // If the heightfield is higher than the ufo, return true. Else return false
	// NB: Will need to add some sort of 'bounding box...'
	// Check the heights of a few squares around the ufo?
	if (heightFieldY > ufoPos[1] - 1.5) {
		return true;
	}
	else {
		return false;
	}
}

// If the ufo is going to move into a forbidden zone, return true
// Otherwise, return false
// TODO: Refactor. Don't need to calculate this twice
static bool anyCollisions(int dirDegrees) {
	// Tests for edge of map
	GLfloat testX = ufoPos[0] +
		cos((dirDegrees + ufoDegrees) * M_PI / 180) * 0.25;
	if (testX < 0 || testX > 100) {
		return true;
	}
	GLfloat testZ = ufoPos[2] +
		sin((dirDegrees + ufoDegrees) * M_PI / 180) * 0.25;
	if (testZ < 0 || testZ > 100) {
		return true;
	}

	// Just using this for the printf
	if (anyTerrainCollisions(testX, testZ)) {
		return true;
	}

	// Test for object collisions (just the flagpole atm)
	setTestUfoBB(testX, testZ);
	if (testUfoBB[0][0] < flagPoleBB[1][0] &&
		testUfoBB[1][0] > flagPoleBB[0][0] &&
		testUfoBB[2][2] < flagPoleBB[1][2] &&
		testUfoBB[1][2] > flagPoleBB[2][2]) {
		return true;
	}
	//if (testUfoBB[0][0] < mountainBB[1][0] &&
	//	testUfoBB[1][0] > mountainBB[0][0] &&
	//	testUfoBB[2][2] < mountainBB[1][2] &&
	//	testUfoBB[1][2] > mountainBB[2][2]) {
	//	return true;
	//}
	return false;
}

/* In most GUI systems we would write just one event handler
   for all kinds of keystrokes. In GLUT, we need one for the
   standard ASCII keys and one for special cursor or function
   style keys that vary from system to system. Because the
   GLUT special key code range overlaps with ASCII lowercase,
   it isn't safe to use the same function for both.        */

static void asciiKey (unsigned char key, int x, int y)
{
  if (key == ESC)
    menuChoice(cmdExit);

  // Toggle between camera movement and ufo movement
  //if (key == '4') {
	 // if (debugMode == true) {
		//  debugMode = false;
		//  showWireFrame = false;
		//  showSpin = false;
		//  hiddenDepth = true;
		//  hiddenCull = true;
		//  clockWise = false;
		//  glEnable(GL_LIGHT1);
	 // }
	 // else {
		//  debugMode = true;
		//  glDisable(GL_LIGHT1);
	 // }
  //}

  if (key =='h') {
	  if (helpMode == false) {
		  helpMode = true;
	  }
	  else {
		  helpMode = false;
	  }
  }

  // Toggle the speed of the day / night cycle
  if (key == '+' && dayNightSpeed < 0.5) {
	  dayNightSpeed += 0.1;
  }
  if (key == '-' && dayNightSpeed >= 0.2) {
	  dayNightSpeed -= 0.1;
  }

  if (debugMode) {
	  if (key == 'w') { showWireFrame = !showWireFrame; }
	  //if (key == ' ') { showSpin = !showSpin; }

	  if (key == 'd') { hiddenDepth = !hiddenDepth; }
	  if (key == 'c') { hiddenCull = !hiddenCull; }
	  if (key == '@') { clockWise = !clockWise; }
	  if (key == '/') { spinIncrement = spinIncrement + 1; if (spinIncrement > 4) { spinIncrement = 0.4f; } }

	  if (key == '=') { if (angle < 50) angle = 90; else angle = 0; }
	  //if (key == '+') { angle = angle + 10; angle = (GLfloat)((int)angle % 360); }
	  //if (key == '-') { angle = angle - 10; angle = (GLfloat)((int)angle % 360); }

	  if (key == '.') { eyePoint[0] = eyePoint[0] + 2; eyePoint[2] = eyePoint[2] + 2; }
	  if (key == ',') { eyePoint[0] = eyePoint[0] - 2; eyePoint[2] = eyePoint[2] - 2; }

	  if (key == 'n') {
		  if (drawNormalsAsLines) {
			  drawNormalsAsLines = false;
		  }
		  else {
			  drawNormalsAsLines = true;
		  }
	  }
  }
  else {
	  if (key == 'f') {
		  flagpole->startAnimation = true;
	  }

	  // Movement system
	  if (key == 'j' && !anyCollisions(180)) {
		  ufoPos[0] += (cos((180 + ufoDegrees) * M_PI / 180) * 0.25);
		  ufoPos[2] += (sin((180 + ufoDegrees) * M_PI / 180) * 0.25);
		  setUfoBB();
	  }
	  if (key == 'l' && !anyCollisions(0)) {
		  ufoPos[0] += (cos((0 + ufoDegrees) * M_PI / 180) * 0.25);
		  ufoPos[2] += (sin((0 + ufoDegrees) * M_PI / 180) * 0.25);
		  setUfoBB();
	  }
	  if (key == 'i' && !anyCollisions(270)) {
		  ufoPos[0] += (cos((270 + ufoDegrees) * M_PI / 180) * 0.25);
		  ufoPos[2] += (sin((270 + ufoDegrees) * M_PI / 180) * 0.25);
		  setUfoBB();
	  }
	  if (key == 'k' && !anyCollisions(90)) {
		  ufoPos[0] += (cos((90 + ufoDegrees) * M_PI / 180) * 0.25);
		  ufoPos[2] += (sin((90 + ufoDegrees) * M_PI / 180) * 0.25);
		  setUfoBB();
	  }
	  // Rotation
	  if (key == 'o') {
		  ufoDegrees += 1;
		  if (ufoDegrees < 0) {
			  ufoDegrees += 360;
		  }
	  }
	  if (key == 'u') {
		  ufoDegrees -= 1;
		  if (ufoDegrees >= 360) {
			  ufoDegrees -= 360;
		  }
	  }

	  // Check whether the ufo is currently hovering over any potential abductables
	  // If it is, flag them as 'abducting'
	  if (key == 'p' && !debugMode) {
		  for (int i = 0; i < numAbductees; i++) {
			  if (abductees[i].notAbducted) {
				  if ((fabs(ufoPos[0] - abductees[i].xPos) < 0.8) && (fabs(ufoPos[2] - abductees[i].zPos) < 0.8)) {
					  abductees[i].notAbducted = false;
					  abductees[i].abducting = true;
					  abductees[i].renderCode = 9;

					  // If all the astronauts are abducting/abducted, raise the alien flag
					  if (i == 0 || i == 1 || i == 2) {
						  if (!abductees[0].notAbducted && !abductees[1].notAbducted &&
							  !abductees[2].notAbducted) {
							  flagpole->flagToRenderNext = 3;
							  flagpole->startAnimation = true;
						  }
					  }

					  // If all the fireworks guys are abducting/abduced, stop letting off more fireworks
					  if (i == 5 || i == 6) {
						  if (!abductees[5].notAbducted && !abductees[6].notAbducted) {
							  particleSystemsStopping = true;
						  }
					  }

					  PlaySound(TEXT("xFiles.wav"), NULL, SND_FILENAME | SND_ASYNC);
				  }
			  }
		  }
	  }
  }
}

void setStartValues()
{
	// These are Rob's original values
  //eyePoint[0] = 12.0;
  //eyePoint[1] = 12.0;
  //eyePoint[2] = 26.0;

  lookAtPoint[0]=0;
  lookAtPoint[1]=0;
  lookAtPoint[2]=0;

  upVec[0]=0;
  upVec[1]=1;
  upVec[2]=0;

  fov=60; // field of view
  nearPlane=0.5;
  farPlane=200;
  
  spinAngle = 0.0;
}

static void specialKey (int key, int x, int y)
{
  
  if (key==GLUT_KEY_F1) {
	  if (debugMode == true) {
		  debugMode = false;
		  showWireFrame = false;
		  showSpin = false;
		  hiddenDepth = true;
		  hiddenCull = true;
		  clockWise = false;
		  glEnable(GL_LIGHT1);
	  }
	  else {
		  debugMode = true;
		  glDisable(GL_LIGHT1);
	  }
  }

  if (key==GLUT_KEY_F2)
  {
  setStartValues();
  }

  if (debugMode) {
	  if (key == GLUT_KEY_LEFT) eyePoint[0] = eyePoint[0] + 2;
	  if (key == GLUT_KEY_RIGHT) eyePoint[0] = eyePoint[0] - 2;
	  if (key == GLUT_KEY_UP) eyePoint[2] = eyePoint[2] - 2;
	  if (key == GLUT_KEY_DOWN) eyePoint[2] = eyePoint[2] + 2;
	  if (key == GLUT_KEY_PAGE_UP) eyePoint[1] = eyePoint[1] + 2;
	  if (key == GLUT_KEY_PAGE_DOWN) eyePoint[1] = eyePoint[1] - 2;
  }
}

void exitFunction(void)
{
  delete axes;
  delete drawText2d; 
  delete grid;

  gluDeleteQuadric(quadric1);
  gluDeleteQuadric(quadric2);
  gluDeleteQuadric(quadric3);
  gluDeleteQuadric(quadric4);
}


/****		Startup			****/

static void initGraphics (void)
{
	/***** Texture *****/
	readTextureFiles();

	/*Lighting stuff*/
	glEnable(GL_LIGHTING);
	// Sets the color and intensity of ambient luight
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, defaultAmbient);

	// Sunlight
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunLightDiffuse);	
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	// Moonlight
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, moonLightDiffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white);

	// Ufo light (cyan). We set everything here except the position, which is determined by the location
	// of the Ufo.
	glLightfv(GL_LIGHT1, GL_DIFFUSE, cyan);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.05);

	glEnable(GL_NORMALIZE);

  /* Black background */
  glClearColor(0, 0, 0, 0);

  /* colour green */
  color3[0]=0;
  color3[0]=1;
  color3[0]=0;

  glEnable(GL_DEPTH_TEST);

  /* Needed for vertex arrays */
  //glEnableClientState(GL_VERTEX_ARRAY);
  
  /* Popup menu attached to right mouse button */
  AppMenu = glutCreateMenu(menuChoice);
  glutSetMenu(AppMenu);
  glutAddMenuEntry("Red", cmdRed);
  glutAddMenuEntry("Green", cmdGreen);
  glutAddMenuEntry("----", 0);
  glutAddMenuEntry("Exit", cmdExit);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  /* Start values */
   setStartValues();

  menuChoice(cmdGreen);

  axes = new Axes();
  drawText2d = new Text2D();
  grid = new Grid();
  grid->setUp(20,20,10,10);
  quadric1=gluNewQuadric();

  quadric2=gluNewQuadric();

  quadric3=gluNewQuadric();
  quadric4=gluNewQuadric();

  domeRoof = new DomeClass(10, 10, 9, 1);
  domeSatellite = new DomeClass(10, 10, 5, 1);
  domeUfoBody = new DomeClass(20, 10, 3, 1);
  domeUfoBubble = new DomeClass(20, 10, 6, 1);
  cube = new CubeClass();
  taperedPrism = new TaperedPrism(0, 0.8);
  truncatedPyramid = new TaperedPrism(0.75, 0.75);
  carFront = new TaperedPrism(0.25, 0.4);
  carMiddle = new TaperedPrism(0.5, 1);
  triangularPrism = new TriangularPrism();
  flagpole = new FlagPole(textureHandles[4], textureHandles[7]);

  // Heightfield stuff
  readHeightField();
  initHeightField();
  scaleHeightField();
  initSurfaceNormals();
  initVertexNormals();
  applyVertexNormals();
  // Test
  separateTrianglesByHeightAndGradient();

  // Alpha blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  setUfoBB();

  atexit (exitFunction);
}


void timer(int value)
{
timeForNextFrame=true;
glutTimerFunc(timerMSecs,timer,0);
}

/****		Main control		****/

// TODO: Factorise the rotation methods
static void idleRoutine(void)
{
	if (timeForNextFrame)
	{
		spinAngle += spinIncrement;
		if (spinAngle > 360.0)
			spinAngle -= 360.0;
		glutPostRedisplay();
		timeForNextFrame = false;

		if (angleVar < angle)
		{
			angleVar = angleVar + 1;
			if (angleVar > angle) angleVar = angle;
		}

		if (angleVar > angle)
		{
			angleVar = angleVar - 1;
			if (angleVar < angle) angleVar = angle;
		}

		// UFO light routine. Also used for landing pad routine
		ufoLight++;
		if (ufoLight >= 150) {
			ufoLight -= 150;
		}

		// UFO land routine
		if (ufoDescending && ufoHeightOffset <= 23.8) {
			// Landing speed
			if (ufoSpeed > 0.1) {
				ufoSpeed -= 0.01;
			}
			ufoHeightOffset += ufoSpeed;
			// Stopping condition
			if (ufoHeightOffset > 23.8) {
				ufoDescending = false;
				ufoLanded = true;
			}
		}
		else if (ufoAscending && ufoHeightOffset > 0) {
			// Takeoff speed
			if (ufoSpeed < 1) {
				ufoSpeed += 0.03;
			}
			ufoHeightOffset -= ufoSpeed;
			if (ufoHeightOffset < 0) {
				ufoAscending = false;
				ufoLanded = false;
			}
		}

		// icosahedron rotation routine
		icoSpinAngle += icoSpinSpeed;
		if (icoSpinAngle > 360) {
			icoSpinAngle -= 360;
		}

		// Satellite rotation routine
		satelliteRotationAngle += satelliteRotationSpeed;
		if (satelliteRotationSpeed > 360) {
			satelliteRotationAngle -= 360;
		}

		// Colour pulsation routine
		if (pulsationIncreasing) {
			pulsationCounter += 4;
			if (pulsationCounter > 255) {
				pulsationIncreasing = false;
			}
		}
		else {
			pulsationCounter -= 4;
			if (pulsationCounter < 0) {
				pulsationCounter = 0;
				pulsationIncreasing = true;
			}
		}

		pulsatingGreen[0] = ((float)90 + (2 * pulsationCounter / 3)) / 255;
		pulsatingGreen[1] = ((float)135 + (pulsationCounter / 2)) / 255;
		pulsatingGreen[2] = ((float)0 + (3 * pulsationCounter / 4)) / 255;

		// If an abductee is currently being abducted, increment their height
		// Additionally, increment their x and z coordinates to 'track' the Ufo
		// Once they get high enough, mark them as vanished.
		for (int i = 0; i < numAbductees; i++) {
			if (abductees[i].abducting) {
					abductees[i].xPos = ufoPos[0];
					abductees[i].yPos += ABDUCTION_SPEED;
					abductees[i].zPos = ufoPos[2];
				if ((abductees[i].isCow && abductees[i].yPos >= ufoPos[1] - 2) ||
					(!abductees[i].isCow && abductees[i].yPos >= ufoPos[1] - 1.5)) {
					abductees[i].abducting = false;
					abductees[i].abducted = true;
					numAbductions++;
				}
			}
		}

		// Day/night cycle - change the direction of LIGHT0 (the sun)
		dayNightDegrees -= dayNightSpeed;
		if (dayNightDegrees <= 0) {
			dayNightDegrees += 360;
		}

		// TEST
		GLfloat dayNightDegreesOffset = dayNightDegrees - 45;

		dayNightTime = (360 - (((int)dayNightDegrees + 45) % 360)) / 15;
		
		light0Pos[2] = cos(dayNightDegreesOffset * M_PI / 180);
		light0Pos[1] = sin(dayNightDegreesOffset * M_PI / 180);

		light2Pos[2] = -light0Pos[2];
		light2Pos[1] = -light0Pos[1];

		// Change the colour of the sky
		if (dayNightDegrees < 240 && dayNightDegrees >= 225) {
			interpolateColor(skyNight, skyOrange, 240, 225, dayNightDegrees, skyColor);
		}
		else if ((dayNightDegrees < 225 && dayNightDegrees >= 210)) {
			interpolateColor(skyOrange, skyDay, 225, 210, dayNightDegrees, skyColor);
		}
		//else if ((dayNightDegrees < 210 && dayNightDegrees >= 195)) {
		//	interpolateSky(skyYellow, skyDay, 210, 195, dayNightDegrees);
		//}
		else if ((dayNightDegrees < 75 && dayNightDegrees >= 45)) {
			interpolateColor(skyDay, skyOrange, 75, 45, dayNightDegrees, skyColor);
		}
		//else if ((dayNightDegrees < 60 && dayNightDegrees >= 45)) {
		//	interpolateSky(skyYellow, skyOrange, 60, 45, dayNightDegrees);
		//}
		else if ((dayNightDegrees < 45 && dayNightDegrees >= 30)) {
			interpolateColor(skyOrange, skyPink, 45, 30, dayNightDegrees, skyColor);
		}
		else if ((dayNightDegrees < 30 && dayNightDegrees >= 15)) {
			interpolateColor(skyPink, skyPurple, 30, 15, dayNightDegrees, skyColor);
		}
		else if ((dayNightDegrees < 15 && dayNightDegrees >= 0)) {
			interpolateColor(skyPurple, skyNight, 15, 0, dayNightDegrees, skyColor);
		}

		// Change the colour of the sun / moon lights
		if (dayNightDegrees < 255 && dayNightDegrees >= 225) {
			interpolateColor(black, sunLightDiffuse, 255, 225, dayNightDegrees, sunLight);
			interpolateColor(moonLightDiffuse, black, 255, 225, dayNightDegrees, moonLight);
		}
		else if (dayNightDegrees < 45 && dayNightDegrees >= 15) {
			interpolateColor(sunLightDiffuse, black, 45, 15, dayNightDegrees, sunLight);
			interpolateColor(black, moonLightDiffuse, 45, 15, dayNightDegrees, moonLight);
		}

		// Particle System timer
		if (particleSystemsRunning) {
			particleTimer += 0.1;
			if ((int)particleTimer == 15) {
				particleSystems[0].isRunningRocket = true;
			}
			else if ((int)particleTimer == 16) {
				particleSystems[1].isRunningRocket = true;
			}
			else if ((int)particleTimer == 17) {
				particleSystems[2].isRunningRocket = true;
			}
			else if (particleTimer > 30) {
				if (particleSystemsStopping) {
					particleSystemsRunning = false;
				}
				particleTimer = 0;
			}
		}

  }

}

int main (int argc, char * argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

  glutInitWindowSize(1000, 800);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Alien Abduction");

  initGraphics();
  
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  
  glutKeyboardFunc(asciiKey);
  glutSpecialFunc(specialKey);
  
  glutIdleFunc(idleRoutine);
  glutTimerFunc(timerMSecs,timer,0);
  
  glutMainLoop();
  /* Should never get here, but keeps compiler happy */
  return 0;
}
