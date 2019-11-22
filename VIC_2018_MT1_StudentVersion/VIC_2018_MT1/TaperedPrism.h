#pragma once
#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

class TaperedPrism {
public:
	TaperedPrism(float x, float y);
	void render();
private:
	float xScale;
	float yScale;
	GLfloat vertices[8][3];
	void CalculateVectorNormal(GLfloat v1[], GLfloat v2[], GLfloat v3[], 
		GLfloat *normalX, GLfloat *normalY, GLfloat *normalZ);
	float nx, ny, nz;
};
