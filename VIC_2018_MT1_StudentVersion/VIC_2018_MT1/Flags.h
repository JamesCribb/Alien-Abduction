#pragma once
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "axes.h"
#include "TexturedPolygons.h"
#include "Colors.h"

#define GLUT_DISABLE_ATEXIT_HACK


class USFlag {
public:
	USFlag();
	void render();
private:
	void drawStar();
	Axes *axes;
};

class ChinaFlag {
public:
	ChinaFlag();
	void render();
};

class TexturedFlag {
public:
	TexturedFlag(GLuint texh);	
	void render(const GLfloat *edgeAmbient, const GLfloat *edgeDiffuse);
private:
	GLuint texture;
};