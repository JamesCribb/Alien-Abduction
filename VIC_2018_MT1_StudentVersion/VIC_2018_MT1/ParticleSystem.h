#pragma once
#include <stdlib.h>
#include <stdio.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "Colors.h"

#define _USE_MATH_DEFINES
#include <math.h>

class ParticleSystem {
public:
	ParticleSystem(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, 
		GLfloat b);
	void resetToFireworks();
	void resetToRocket();
	void renderFireworks();
	void renderRocket();
	bool isRunningFireworks;
	bool isRunningRocket;
	GLfloat psPosition[3];
	GLfloat initialYPos;
	GLfloat timer;
private: 
	struct Particle {
		GLfloat position[3];
		GLfloat vector[3];
	};
	Particle particles[100];
	GLfloat targetColor[3];
	GLfloat color[3];
	
};