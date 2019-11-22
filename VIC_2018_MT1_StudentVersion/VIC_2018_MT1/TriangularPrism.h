#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

class TriangularPrism {
public:
	TriangularPrism();
	void render();
};