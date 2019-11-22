#include "TriangularPrism.h"

// In theory, this should be centered, but that involves trig...
static GLfloat Vertices[6][3] = {
	{0, 0, 0},	// 0	bottom left front
	{1, 0, 0},	// 1	front right
	{0, 1, 0},	// 2	top left front
	{0, 0, -1},	// 3	bottom left back
	{1, 0, -1},	// 4	back right
	{0, 1, -1}	// 5	top left back
};

static GLuint TriangularFaces[] = {
	0, 1, 2,	// front
	3, 4, 5,	// back
};

static GLuint SquareFaces[] = {
	0, 3, 5, 2,	// left
	0, 1, 4, 3, // bottom
	1, 4, 5, 2	// hypotenuse
};

TriangularPrism::TriangularPrism() {

}

void TriangularPrism::render() {

	glBegin(GL_QUADS);
	for (int i = 0; i < 3 * 4; i++) {
		glVertex3fv(Vertices[SquareFaces[i]]);
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 2 * 3; i++) {
		glVertex3fv(Vertices[TriangularFaces[i]]);
	}
	glEnd();
}