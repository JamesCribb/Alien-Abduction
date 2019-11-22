#pragma once
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Flags.h"
#include "axes.h"

#define GLUT_DISABLE_ATEXIT_HACK


class FlagPole {
public:
	FlagPole(GLuint texh1, GLuint texh2);
	void render();
	bool startAnimation;
	int flagToRender;
	int flagToRenderNext;
private:
	Axes *axes;
	USFlag *usFlag;
	ChinaFlag *chinaFlag;
	TexturedFlag *ufpFlag;
	TexturedFlag *alienFlag;
	GLUquadric *quadric;
};