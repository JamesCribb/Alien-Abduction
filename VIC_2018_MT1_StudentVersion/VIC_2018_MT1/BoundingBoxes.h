#pragma once
#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

// Flagpole
GLfloat flagPoleBB[8][3] = {
	{14.8, 4.5, 35.2}, // bottom front left
	{15.2, 4.5, 35.2}, // bottom front right
	{15.2, 4.5, 34.8}, // bottom back right
	{14.8, 4.5, 34.8}, // bottom back left
	{14.8, 13, 35.2}, // top front left
	{15.2, 13, 35.2}, // top front right
	{15.2, 13, 34.8}, // top back right
	{14.8, 13, 34.8} // top back left
};

// Mountain(s). Try without y, since the change of the ufo ever ascending is vey low
GLfloat mountainBB[4][3] = {
	{43, -1, 27}, {56, -1, 27}, {56, -1, 17.5}, {43, -1, 17.5}
};