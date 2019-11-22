/* Contains methods for reading heightfields from file, adding normals, and rendering */

#pragma once
#include <iostream>
#include <fstream>
#include "glut.h"
#include <GL/gl.h>
#include <GL/glu.h>

#define GLUT_DISABLE_ATEXIT_HACK

#include "Colors.h"

#define SIZE 400	// size of the heightfield. It has to be a square.

// Holds height data that is read in from the file
static GLfloat heightFieldArray[SIZE][SIZE];

// Holds the x, y, z coordinates of each vertex normal in the heightfield
static GLfloat vertexNormals[SIZE][SIZE][3];

// Holds info from which the triangles that make up the heightfield are rendered
// 0, 1, 2: vertices of the triangle
// 3: surface normal
// 4, 5, 6: vertex normals of 0, 1 and 2, respectively
static GLfloat triangles[(SIZE - 1) * (SIZE - 1) * 2][7][3];

// Holds only those triangles that actually represent terrain, not the (flat) sea floor
// Should be faster to render...
static GLfloat terrainTriangles[196002][7][3];

// Separate arrays for triangles of different texture types
static GLfloat beachTriangles[28152][7][3];
static GLfloat cliffTriangles[24702][7][3];
static GLfloat grassTriangles[143148][7][3];

// Reads the heightfield data from a file and stores it in heightFieldArray
void readHeightField() {
	std::ifstream hfFile("santoriniSmallTestNumbers.txt");
	int num;
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			hfFile >> num;
			heightFieldArray[y][x] = (GLfloat)num;
		}
	}
}

// Initialises the vertex coordinates of the triangles used to draw the heightfield
void initHeightField() {
	for (int y = 0; y < SIZE - 1; y++) {
		for (int x = 0; x < SIZE - 1; x++) {
			// first triangle
			triangles[y * ((SIZE - 1) * 2) + x * 2][0][0] = y;
			triangles[y * ((SIZE - 1) * 2) + x * 2][0][1] = heightFieldArray[y][x];
			triangles[y * ((SIZE - 1) * 2) + x * 2][0][2] = x;
			triangles[y * ((SIZE - 1) * 2) + x * 2][1][0] = y;
			triangles[y * ((SIZE - 1) * 2) + x * 2][1][1] = heightFieldArray[y][x + 1];
			triangles[y * ((SIZE - 1) * 2) + x * 2][1][2] = x + 1;
			triangles[y * ((SIZE - 1) * 2) + x * 2][2][0] = y + 1;
			triangles[y * ((SIZE - 1) * 2) + x * 2][2][1] = heightFieldArray[y + 1][x + 1];
			triangles[y * ((SIZE - 1) * 2) + x * 2][2][2] = x + 1;
			// second triangle
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][0][0] = y;
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][0][1] = heightFieldArray[y][x];
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][0][2] = x;
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][1][0] = y + 1;
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][1][1] = heightFieldArray[y + 1][x + 1];
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][1][2] = x + 1;
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][2][0] = y + 1;
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][2][1] = heightFieldArray[y + 1][x];
			triangles[y * ((SIZE - 1) * 2) + (x * 2 + 1)][2][2] = x;
		}
	}
}

// Scale the heightfield so that it's in a more convenient form
void scaleHeightField() {
	for (int i = 0; i < (SIZE - 1) * (SIZE - 1) * 2; i++) {
		triangles[i][0][0] *= 0.2;
		triangles[i][0][1] *= 0.05;
		triangles[i][0][2] *= 0.2;
		triangles[i][1][0] *= 0.2;
		triangles[i][1][1] *= 0.05;
		triangles[i][1][2] *= 0.2;
		triangles[i][2][0] *= 0.2;
		triangles[i][2][1] *= 0.05;
		triangles[i][2][2] *= 0.2;
	}
}

// Uses code modified from https://support.microsoft.com/en-au/help/131130/how-to-set-the-current-normal-vector-in-an-opengl-application
void initSurfaceNormals() {
	
	for (int i = 0; i < (SIZE - 1) * (SIZE - 1) * 2; i++) {

		GLfloat Qx, Qy, Qz, Px, Py, Pz;

		Qx = triangles[i][1][0] - triangles[i][0][0];
		Qy = triangles[i][1][1] - triangles[i][0][1];
		Qz = triangles[i][1][2] - triangles[i][0][2];

		Px = triangles[i][2][0] - triangles[i][0][0];
		Py = triangles[i][2][1] - triangles[i][0][1];
		Pz = triangles[i][2][2] - triangles[i][0][2];

		// Set the normal vector
		triangles[i][3][0] = -(Py * Qz - Pz * Qy);
		triangles[i][3][1] = -(Pz * Qx - Px * Qz);
		triangles[i][3][2] = -(Px * Qy - Py * Qx);

		// Hack to fix strange rendering artefact
		if (i % 798 > 780) {
			triangles[i][3][0] = 0;
			triangles[i][3][1] = 1;
			triangles[i][3][2] = 0;
		}
	}
}

void initVertexNormals() {
	int i;
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			// CORNERS
			// TOP LEFT
			if (y == 0 && x == 0) {
				vertexNormals[y][x][0] = (triangles[0][3][0] +
					triangles[1][3][0]) / 2;
				vertexNormals[y][x][1] = (triangles[0][3][1] +
					triangles[1][3][1]) / 2;
				vertexNormals[y][x][2] = (triangles[0][3][2] +
					triangles[1][3][2]) / 2;
			}
			// BOTTOM RIGHT
			else if (y == SIZE - 1 && x == SIZE - 1) {
				i = (SIZE - 1) * (SIZE - 1) * 2;
				vertexNormals[y][x][0] = (triangles[i - 2][3][0] +
					triangles[i - 1][3][0]) / 2;
				vertexNormals[y][x][1] = (triangles[i - 2][3][1] +
					triangles[i - 1][3][1]) / 2;
				vertexNormals[y][x][2] = (triangles[i - 2][3][2] +
					triangles[i - 1][3][2]) / 2;
			}
			// TOP RIGHT
			else if (y == 0 && x == SIZE - 1) {
				i = ((SIZE - 1) * 2) - 2;
				vertexNormals[y][x][0] = triangles[i][3][0];
				vertexNormals[y][x][1] = triangles[i][3][1];
				vertexNormals[y][x][2] = triangles[i][3][2];
			}
			// BOTTOM LEFT
			else if (y == SIZE - 1 && x == 0) {
				i = (SIZE - 1) * (SIZE - 2) * 2;
				vertexNormals[y][x][0] = triangles[i][3][0];
				vertexNormals[y][x][1] = triangles[i][3][1];
				vertexNormals[y][x][2] = triangles[i][3][2];
			}
			// EDGES
			// TOP
			else if (y == 0) {
				i = (x - 1) * 2;
				vertexNormals[y][x][0] = (triangles[i][3][0] +
					triangles[i + 3][3][0] +
					triangles[i + 2][3][0]) / 3;
				vertexNormals[y][x][1] = (triangles[i][3][1] +
					triangles[i + 3][3][1] +
					triangles[i + 2][3][1]) / 3;
				vertexNormals[y][x][2] = (triangles[i][3][2] +
					triangles[i + 3][3][2] +
					triangles[i + 2][3][2]) / 3;
			}
			// BOTTOM
			else if (y == SIZE - 1) {
				i = ((SIZE - 1) * (SIZE - 2) * 2) + (x * 2 + 1);
				vertexNormals[y][x][0] = (triangles[i][3][0] +
					triangles[i - 1][3][0] +
					triangles[i + 2][3][0]) / 3;
				vertexNormals[y][x][1] = (triangles[i][3][1] +
					triangles[i - 1][3][1] +
					triangles[i + 2][3][1]) / 3;
				vertexNormals[y][x][2] = (triangles[i][3][2] +
					triangles[i - 1][3][2] +
					triangles[i + 2][3][2]) / 3;
			}
			// LEFT
			else if (x == 0) {
				i = ((SIZE - 1) * 2 * (y - 1)) + 1;
				vertexNormals[y][x][0] = (triangles[i][3][0] +
					triangles[i + 11][3][0] +
					triangles[i + 12][3][0]) / 3;
				vertexNormals[y][x][1] = (triangles[i][3][1] +
					triangles[i + 11][3][1] +
					triangles[i + 12][3][1]) / 3;
				vertexNormals[y][x][2] = (triangles[i][3][2] +
					triangles[i + 11][3][2] +
					triangles[i + 12][3][2]) / 3;
			}
			// RIGHT
			else if (x == SIZE - 1) {
				i = ((SIZE - 1) * 2 * y) - 2;
				vertexNormals[y][x][0] = (triangles[i][3][0] +
					triangles[i + 1][3][0] +
					triangles[i + 12][3][0]) / 3;
				vertexNormals[y][x][1] = (triangles[i][3][1] +
					triangles[i + 1][3][1] +
					triangles[i + 12][3][1]) / 3;					
				vertexNormals[y][x][2] = (triangles[i][3][2] +
					triangles[i + 1][3][2] +		
					triangles[i + 12][3][2]) / 3;
			}
			// EVERYTHING ELSE
			else {
				i = ((2 * (SIZE - 1)) * (y - 1)) + ((x * 2) - 1);
				vertexNormals[y][x][0] = (triangles[i][3][0] + 
					triangles[i - 1][3][0] + 
					triangles[i + 2][3][0] + 
					triangles[i + 13][3][0] + 
					triangles[i + 14][3][0] + 
					triangles[i + 11][3][0]) / 6;
				vertexNormals[y][x][1] = (triangles[i][3][1] +
					triangles[i - 1][3][1] +
					triangles[i + 2][3][1] +
					triangles[i + 13][3][1] +
					triangles[i + 14][3][1] +
					triangles[i + 11][3][1]) / 6;
				vertexNormals[y][x][2] = (triangles[i][3][2] +
					triangles[i - 1][3][2] +
					triangles[i + 2][3][2] +
					triangles[i + 13][3][2] +
					triangles[i + 14][3][2] +
					triangles[i + 11][3][2]) / 6;
			}
		}
	}
}

// Associate the vertex normals with the relevant vertices for each triangle
void applyVertexNormals() {
	for (int y = 0; y < SIZE - 1; y++) {
		for (int x = 0; x < SIZE - 1; x++) {
			// first triangle
			triangles[y * (2 * (SIZE - 1)) + x * 2][4][0] = vertexNormals[y][x][0];
			triangles[y * (2 * (SIZE - 1)) + x * 2][4][1] = vertexNormals[y][x][1];
			triangles[y * (2 * (SIZE - 1)) + x * 2][4][2] = vertexNormals[y][x][2];
			triangles[y * (2 * (SIZE - 1)) + x * 2][5][0] = vertexNormals[y][x + 1][0];
			triangles[y * (2 * (SIZE - 1)) + x * 2][5][1] = vertexNormals[y][x + 1][1];
			triangles[y * (2 * (SIZE - 1)) + x * 2][5][2] = vertexNormals[y][x + 1][2];
			triangles[y * (2 * (SIZE - 1)) + x * 2][6][0] = vertexNormals[y + 1][x + 1][0];
			triangles[y * (2 * (SIZE - 1)) + x * 2][6][1] = vertexNormals[y + 1][x + 1][1];
			triangles[y * (2 * (SIZE - 1)) + x * 2][6][2] = vertexNormals[y + 1][x + 1][2];
			// second triangle
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][4][0] = vertexNormals[y][x][0];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][4][1] = vertexNormals[y][x][1];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][4][2] = vertexNormals[y][x][2];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][5][0] = vertexNormals[y + 1][x + 1][0];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][5][1] = vertexNormals[y + 1][x + 1][1];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][5][2] = vertexNormals[y + 1][x + 1][2];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][6][0] = vertexNormals[y + 1][x][0];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][6][1] = vertexNormals[y + 1][x][1];
			triangles[y * (2 * (SIZE - 1)) + (x * 2 + 1)][6][2] = vertexNormals[y + 1][x][2];
		}
	}
}

GLfloat getSlope(GLfloat y1, GLfloat y2, GLfloat y3) {
	GLfloat largest, smallest;
	if (y1 >= y2 && y1 >= y3) {
		largest = y1;
		if (y2 <= y3) {
			smallest = y2;
		}
		else {
			smallest = y3;
		}
	}
	else if (y2 >= y1 && y2 >= y3) {
		largest = y2;
		if (y1 <= y3) {
			smallest = y1;
		}
		else {
			smallest = y3;
		}
	}
	else {
		largest = y3;
		if (y1 <= y2) {
			smallest = y1;
		}
		else {
			smallest = y2;
		}
	}

	return largest - smallest;
}

GLfloat getAverageHeight(GLfloat y1, GLfloat y2, GLfloat y3) {
	return (y1 + y2 + y3) / 3;
}

void copyTriangle(GLfloat source[][7][3], GLfloat destination[][7][3], int counter, int sourceIndex) {

	destination[counter][0][0] = source[sourceIndex][0][0];
	destination[counter][0][1] = source[sourceIndex][0][1];
	destination[counter][0][2] = source[sourceIndex][0][2];

	destination[counter][1][0] = source[sourceIndex][1][0];
	destination[counter][1][1] = source[sourceIndex][1][1];
	destination[counter][1][2] = source[sourceIndex][1][2];

	destination[counter][2][0] = source[sourceIndex][2][0];
	destination[counter][2][1] = source[sourceIndex][2][1];
	destination[counter][2][2] = source[sourceIndex][2][2];

	destination[counter][3][0] = source[sourceIndex][3][0];
	destination[counter][3][1] = source[sourceIndex][3][1];
	destination[counter][3][2] = source[sourceIndex][3][2];

	destination[counter][4][0] = source[sourceIndex][4][0];
	destination[counter][4][1] = source[sourceIndex][4][1];
	destination[counter][4][2] = source[sourceIndex][4][2];

	destination[counter][5][0] = source[sourceIndex][5][0];
	destination[counter][5][1] = source[sourceIndex][5][1];
	destination[counter][5][2] = source[sourceIndex][5][2];

	destination[counter][6][0] = source[sourceIndex][6][0];
	destination[counter][6][1] = source[sourceIndex][6][1];
	destination[counter][6][2] = source[sourceIndex][6][2];
}

void drawHeightField(GLuint grassTexH, GLuint stoneTexH) {

	// Draw the beach
	glMaterialfv(GL_FRONT, GL_AMBIENT, landAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, land);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 28152; i++) {
		glNormal3fv(beachTriangles[i][4]);
		glVertex3fv(beachTriangles[i][0]);
		glNormal3fv(beachTriangles[i][5]);
		glVertex3fv(beachTriangles[i][1]);
		glNormal3fv(beachTriangles[i][6]);
		glVertex3fv(beachTriangles[i][2]);
	}
	glEnd();

	// Draw the grass
	glMaterialfv(GL_FRONT, GL_AMBIENT, grassAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grass);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 143148; i++) {
		glTexCoord2f(1, 0);
		glNormal3fv(grassTriangles[i][4]);
		glVertex3fv(grassTriangles[i][0]);
		glTexCoord2f(1, 1);
		glNormal3fv(grassTriangles[i][5]);
		glVertex3fv(grassTriangles[i][1]);
		glTexCoord2f(0, 1);
		glNormal3fv(grassTriangles[i][6]);
		glVertex3fv(grassTriangles[i][2]);
	}
	glEnd();

	// Draw the cliffs
	glMaterialfv(GL_FRONT, GL_AMBIENT, stoneAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 143148; i++) {
		glNormal3fv(cliffTriangles[i][4]);
		glVertex3fv(cliffTriangles[i][0]);
		glNormal3fv(cliffTriangles[i][5]);
		glVertex3fv(cliffTriangles[i][1]);
		glNormal3fv(cliffTriangles[i][6]);
		glVertex3fv(cliffTriangles[i][2]);
	}
	glEnd();
}

void drawHeightFieldNormalsAsLines() {

	glDisable(GL_LIGHTING);
	glColor3fv(blue);

	//// Vertex normals as points
	//for (int i = 0; i < (SIZE - 1) * (SIZE - 1) * 2; i++) {
	//	// Not exactly the middle of the triangle, but close enough...
	//	glPushMatrix();
	//	glTranslatef(triangles[i][0][0], triangles[i][0][1], triangles[i][0][2]);
	//	glTranslatef(0, 1, 0);
	//	glBegin(GL_POINTS);
	//	glVertex3f(0, 0, 0);
	//	glEnd();
	//	glPopMatrix();
	//}

	// Vertex normals as lines
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			glPushMatrix();
			glTranslatef(j * 0.20, heightFieldArray[j][i] * 0.05, i * 0.20);
			glBegin(GL_LINES);
			glVertex3f(0, 0, 0);
			glVertex3f(vertexNormals[j][i][0], vertexNormals[j][i][1], vertexNormals[j][i][2]);
			glEnd();
			glPopMatrix();
		}
	}

	glEnable(GL_LIGHTING);
}

// Iterates through the main triangle array, finding all non-flat triangles
// Puts these triangles in a new array so that they render faster
void separateTrianglesByHeightAndGradient() {
	
	int beachTrianglesCounter = 0;
	int cliffTrianglesCounter = 0;
	int grassTrianglesCounter = 0;

	for (int i = 0; i < (SIZE - 1) * (SIZE - 1) * 2; i++) {
		if (!(triangles[i][0][1] == 0 && triangles[i][1][1] == 0 && triangles[i][2][1] == 0)) {
			if (getAverageHeight(triangles[i][0][1], triangles[i][1][1], triangles[i][2][1]) < 0.8) {
				// Put the triangle in beachTriangles
				copyTriangle(triangles, beachTriangles, beachTrianglesCounter, i);
				beachTrianglesCounter++;
			}
			else {
				if (getSlope(triangles[i][0][1], triangles[i][1][1],
					triangles[i][2][1]) > 0.17) {
					// Put the triangle in cliffTriangles
					copyTriangle(triangles, cliffTriangles, cliffTrianglesCounter, i);
					cliffTrianglesCounter++;
				}
				else {
					// Put the triangle in grassTriangles
					copyTriangle(triangles, grassTriangles, grassTrianglesCounter, i);
					grassTrianglesCounter++;
				}
			}
		}
	}
}

// If the y value of each of a triangle's vertices is zero, it is flat and can be ignored
void identifyTriangleTypes() {

	int numBeachTriangles = 0;
	int numGrassTriangles = 0;
	int numCliffTriangles = 0;

	for (int i = 0; i < (SIZE - 1) * (SIZE - 1) * 2; i++) {
		if (!(triangles[i][0][1] == 0 && triangles[i][1][1] == 0 && triangles[i][2][1] == 0)) {
			if (getAverageHeight(triangles[i][0][1], triangles[i][1][1], triangles[i][2][1]) < 0.8) {
				numBeachTriangles++;
			}
			else {
				if (getSlope(triangles[i][0][1], triangles[i][1][1],
					triangles[i][2][1]) > 0.17) {
					numCliffTriangles++;
				}
				else {
					numGrassTriangles++;
				}
			}
		}
	}

	printf("Beach triangles: %d\nGrass Triangles: %d\nCliff Triangles: %d\n", numBeachTriangles,
		numGrassTriangles, numCliffTriangles);
	printf("Total: %d", numBeachTriangles + numGrassTriangles + numCliffTriangles);
}