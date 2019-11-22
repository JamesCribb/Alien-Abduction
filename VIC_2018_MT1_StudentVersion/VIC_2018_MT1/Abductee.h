#pragma once
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Colors.h"
#include "TexturedPolygons.h"
#include "Dome.h"

#define GLUT_DISABLE_ATEXIT_HACK


class Abductee {
public:
	bool notAbducted;
	bool abducting;
	bool abducted;
	bool centred;
	bool isHuman;
	bool isCow;
	bool isAstronaut;
	char renderCode; // Used to specify the render routine

	// For binding textures in the render routine
	GLuint texh1;
	GLuint texh2;

	GLfloat xPos;
	GLfloat yPos;
	GLfloat initialyPos;
	GLfloat zPos;

	GLfloat angle;	// Used to keep track of rotations

	Abductee(GLfloat x, GLfloat y, GLfloat z, bool human, bool cow, bool astronaut, 
		char rc, GLuint texh1, GLuint texh2);
	void render();

private:
	GLUquadric *quadric;
	void renderHuman();
	void renderCow();
	void renderAstronaut();
	void drawCylinderWithLid();
	void drawHelmetAndVisor();
	void drawAntenna(float needleLength, float needleRadius, float topBitRadius,
		const GLfloat *topBitColor, const GLfloat *topBitAmbient,
		const GLfloat *needleColor, const GLfloat *needleAmbient);

	GLUquadric *quadric5;
	DomeClass *domeHemiSphere;

	GLfloat alpha;
};