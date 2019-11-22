#include "TaperedPrism.h"

static GLuint Faces[] = {
	4, 5, 6, 7,	/* front */
	5, 1, 2, 6,	/* right */
	0, 4, 7, 3,	/* left */
	4, 0, 1, 5,	/* top */
	7, 6, 2, 3,	/* bottom */
	1, 0, 3, 2	/* rear */
};

// https://support.microsoft.com/en-au/help/131130/how-to-set-the-current-normal-vector-in-an-opengl-application
void TaperedPrism::CalculateVectorNormal(GLfloat fVert1[], GLfloat fVert2[],

	GLfloat fVert3[], GLfloat *fNormalX,
	GLfloat *fNormalY, GLfloat *fNormalZ)
{
	GLfloat Qx, Qy, Qz, Px, Py, Pz;

	Qx = fVert2[0] - fVert1[0];
	Qy = fVert2[1] - fVert1[1];
	Qz = fVert2[2] - fVert1[2];
	Px = fVert3[0] - fVert1[0];
	Py = fVert3[1] - fVert1[1];
	Pz = fVert3[2] - fVert1[2];

	*fNormalX = Py * Qz - Pz * Qy;
	*fNormalY = Pz * Qx - Px * Qz;
	*fNormalZ = Px * Qy - Py * Qx;

}

TaperedPrism::TaperedPrism(float x, float y) {

	xScale = x > 1 ? 1 : x;
	yScale = y > 1 ? 1 : y;
	y *= 2;

	vertices[0][0] = -1 + x;	// 0 left top rear
	vertices[0][1] = 1;
	vertices[0][2] = -1;

	vertices[1][0] = 1 - x;		// 1 right top rear
	vertices[1][1] = 1;
	vertices[1][2] = -1;

	vertices[2][0] = 1 - x;		// 2 right bottom rear
	vertices[2][1] = -1 + y;
	vertices[2][2] = -1;

	vertices[3][0] = -1 + x;	// 3 left bottom rear
	vertices[3][1] = -1 + y;
	vertices[3][2] = -1;

	vertices[4][0] = -1;	// 4 left top front
	vertices[4][1] = 1;
	vertices[4][2] = 1;

	vertices[5][0] = 1;		// 5 right top front
	vertices[5][1] = 1;
	vertices[5][2] = 1;

	vertices[6][0] = 1;		// 6 right bottom front
	vertices[6][1] = -1;
	vertices[6][2] = 1;

	vertices[7][0] = -1;	// left bottom front
	vertices[7][1] = -1;
	vertices[7][2] = 1;

}

void TaperedPrism::render() {

	glBegin(GL_QUADS);
	for (int i = 23; i >= 0; i--) {
		if (i % 4 == 0) {
			CalculateVectorNormal(vertices[Faces[i]], vertices[Faces[i + 1]], vertices[Faces[i + 2]],
				&nx, &ny, &nz);
			glNormal3f(nx, ny, nz);
		}
		glVertex3fv(vertices[Faces[i]]);
	}
	glEnd();

}