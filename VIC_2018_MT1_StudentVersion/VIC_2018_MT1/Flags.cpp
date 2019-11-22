#include "Flags.h"
#include "Colors.h"
#define _USE_MATH_DEFINES
#include <math.h>

// TODO: Factorise more nicely (eg, drawStar)

USFlag::USFlag() {
	axes = new Axes();
}

void USFlag::drawStar() {

	glBegin(GL_TRIANGLES);

	glNormal3f(0, 0, 1);
	glVertex3f(cos(306 * M_PI / 180), sin(306 * M_PI / 180), 0);
	glVertex3f(0, 1, 0);
	glVertex3f(cos(198 * M_PI / 180) * 0.39, sin(198 * M_PI / 180) * 0.39, 0);

	glNormal3f(0, 0, 1);
	glVertex3f(cos(234 * M_PI / 180), sin(234 * M_PI / 180), 0);
	glVertex3f(cos(18 * M_PI / 180), sin(18 * M_PI / 180), 0);
	glVertex3f(cos(126 * M_PI / 180) * 0.39, sin(126 * M_PI / 180) * 0.39, 0);

	glNormal3f(0, 0, 1);
	glVertex3f(cos(270 * M_PI / 180) * 0.39, sin(270 * M_PI / 180) * 0.39, 0);
	glVertex3f(cos(18 * M_PI / 180), sin(18 * M_PI / 180), 0);
	glVertex3f(cos(162 * M_PI / 180), sin(162 * M_PI / 180), 0);
	
	glEnd();
}

void USFlag::render() {

	// Blue bit
	glPushMatrix();
	glTranslatef(-7.41, 0, -3);
	glScalef(10.45, 1, 7);
	glMaterialfv(GL_FRONT, GL_AMBIENT, oldGloryBlueAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, oldGloryBlue);
	glutSolidCube(1);
	glPopMatrix();

	// Stars 5 * 6
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteFlagAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);

	glTranslatef(-12, 0, -5.75);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			glPushMatrix();
			glTranslatef(1.7 * i, 0, 1.3 * j);
			glTranslatef(0.4, 0.6, 0);
			glRotatef(90, 1, 0, 0);
			glRotatef(180, 0, 0, 1);
			// Draw the star on one side of the flag
			glPushMatrix();
			glScalef(0.4, 0.4, 1);
			// I think this is necessary for the normals...
			glRotatef(180, 0, 1, 0);
			drawStar();
			glPopMatrix();
			// Draw the star on the other side of the flag
			glTranslatef(0, 0, 1.2);
			glPushMatrix();
			glScalef(0.4, 0.4, 1);
			drawStar();
			glPopMatrix();
			glPopMatrix();
		}
	}
	glPopMatrix();

	// Stars 4 * 5
	glPushMatrix();
	glTranslatef(-11.1, 0, -5);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			glPushMatrix();
			glTranslatef(1.7 * i, 0, 1.3 * j);
			glTranslatef(0.4, 0.6, 0);
			glRotatef(90, 1, 0, 0);
			glRotatef(180, 0, 0, 1);
			// Draw the star on one side of the flag
			glPushMatrix();
			glScalef(0.4, 0.4, 1);
			// I think this is necessary for the normals...
			glRotatef(180, 0, 1, 0);
			drawStar();
			glPopMatrix();
			// Draw the star on the other side of the flag
			glTranslatef(0, 0, 1.2);
			glPushMatrix();
			glScalef(0.4, 0.4, 1);
			drawStar();
			glPopMatrix();

			glPopMatrix();
		}
	}
	glPopMatrix();

	// Top seven stripes
	glPushMatrix();
	glTranslatef(4.94, 0, -6);
	for (int i = 0; i < 7; i++) {
		if (i % 2 == 0) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, oldGloryRedAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, oldGloryRed);
		}
		else {
			glMaterialfv(GL_FRONT, GL_AMBIENT, whiteFlagAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		}
		glPushMatrix();
		glTranslatef(0, 0, i);
		glScalef(14.32, 1, 1);
		glutSolidCube(1);
		glPopMatrix();
	}
	glPopMatrix();

	// Bottom six stripes
	glPushMatrix();
	glTranslatef(-0.25, 0, 1);
	for (int i = 0; i < 6; i++) {
		if (i % 2 == 0) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, whiteFlagAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
		}
		else {
			glMaterialfv(GL_FRONT, GL_AMBIENT, oldGloryRedAmbient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, oldGloryRed);
		}
		glPushMatrix();
		glTranslatef(0, 0, i);
		glScalef(24.7, 1, 1);
		glutSolidCube(1);
		glPopMatrix();
	}

	glPopMatrix();
}

void drawStar() {

	glBegin(GL_TRIANGLES);

	glNormal3f(0, 0, 1);
	glVertex3f(cos(306 * M_PI / 180), sin(306 * M_PI / 180), 0);
	glVertex3f(0, 1, 0);
	glVertex3f(cos(198 * M_PI / 180) * 0.39, sin(198 * M_PI / 180) * 0.39, 0);

	glNormal3f(0, 0, 1);
	glVertex3f(cos(234 * M_PI / 180), sin(234 * M_PI / 180), 0);
	glVertex3f(cos(18 * M_PI / 180), sin(18 * M_PI / 180), 0);
	glVertex3f(cos(126 * M_PI / 180) * 0.39, sin(126 * M_PI / 180) * 0.39, 0);

	glNormal3f(0, 0, 1);
	glVertex3f(cos(270 * M_PI / 180) * 0.39, sin(270 * M_PI / 180) * 0.39, 0);
	glVertex3f(cos(18 * M_PI / 180), sin(18 * M_PI / 180), 0);
	glVertex3f(cos(162 * M_PI / 180), sin(162 * M_PI / 180), 0);

	glEnd();
}

ChinaFlag::ChinaFlag() {

}

void ChinaFlag::render() {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redFlagAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glScalef(24.7, 1, 13);		
	glutSolidCube(1);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowFlagAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
	glTranslatef(-9, 0, -3.5);
	// Big star
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	// Because the star is just a 2D shape, we'll have to draw one on each side of the flag
	glTranslatef(0, 0, -0.6);
	glPushMatrix();
	glScalef(1.7, 1.7, 1);
	// Rotate so that the normal is pointing in the right direction
	glRotatef(180, 0, 1, 0);
	drawStar();
	glPopMatrix();
	glTranslatef(0, 0, 1.2);
	glPushMatrix();
	glScalef(1.7, 1.7, 1);
	drawStar();
	glPopMatrix();
	glPopMatrix(); // end big star

	// Small stars
	glPushMatrix();
	glRotatef(90, 1, 0, 0);	// x right, y down, z back
	glRotatef(180, 0, 0, 1); // x left, y up, z back
	glTranslatef(-2.5, 2, -0.6);
	// top
	glPushMatrix();
	glScalef(0.6, 0.6, 1);
	glRotatef(180, 0, 1, 0);
	drawStar();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1.2);
	glScalef(0.6, 0.6, 1);
	drawStar();
	glPopMatrix();
	// second
	glTranslatef(-1, -1.2, 0);
	glPushMatrix();
	glScalef(0.6, 0.6, 1);
	glRotatef(180, 0, 1, 0);
	drawStar();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1.2);
	glScalef(0.6, 0.6, 1);
	drawStar();
	glPopMatrix();
	// third
	glTranslatef(0, -1.4, 0);
	glPushMatrix();
	glScalef(0.6, 0.6, 1);
	glRotatef(180, 0, 1, 0);
	drawStar();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1.2);
	glScalef(0.6, 0.6, 1);
	drawStar();
	glPopMatrix();
	// bottom
	glTranslatef(1, -1.4, 0);
	glPushMatrix();
	glScalef(0.6, 0.6, 1);
	glRotatef(180, 0, 1, 0);
	drawStar();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 1.2);
	glScalef(0.6, 0.6, 1);
	drawStar();
	glPopMatrix();

	glPopMatrix();
}

TexturedFlag::TexturedFlag(GLuint texh) {
	texture = texh;
}

void TexturedFlag::render(const GLfloat *edgeAmbient, const GLfloat *edgeDiffuse) {

	glMaterialfv(GL_FRONT, GL_EMISSION, verydarkgrey);

	glPushMatrix();
	glScalef(24.7, 1, 13);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	drawCubeTwoSidesTextured(1, edgeAmbient, edgeDiffuse);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);

}