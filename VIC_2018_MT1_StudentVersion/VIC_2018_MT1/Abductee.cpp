#include "Abductee.h"

/*
	RENDER CODES:
		0:	Stationary
		1:	Spinning (test)
*/

Abductee::Abductee(GLfloat x, GLfloat y, GLfloat z, bool human, bool cow, bool astronaut,
	char rc, GLuint th1, GLuint th2) {
	notAbducted = true;
	abducting = false;
	abducted = false;
	centred = false;

	xPos = x;
	yPos = y;
	initialyPos = y;
	zPos = z;

	texh1 = th1;
	texh2 = th2;

	angle = 0;

	isHuman = human;
	isCow = cow;
	isAstronaut = astronaut;

	renderCode = rc;

	quadric5 = gluNewQuadric();
	domeHemiSphere = new DomeClass(10, 10, 9, 1);

	alpha = 1;
}

void Abductee::render() {

	glPushMatrix();
	glTranslatef(xPos, 0.5 + yPos, zPos);
	if (renderCode == 9) {  // spin
		glRotatef(angle, 0, 1, 0);
		angle += 1;
		if (angle > 360) {
			angle -= 360;
		}
	}
	else if (renderCode == 1) { // rotate 45
		glRotatef(45, 0, 1, 0);
		angle = 45;
	}
	else if (renderCode == 2) { // rotate -45
		glRotatef(-45, 0, 1, 0);
		angle = -45;
	}
	else if (renderCode == 3) { // rotate 180
		glRotatef(180, 0, 1, 0);
		angle = 180;
	}

	if (isHuman) {
		renderHuman();
	}
	else if (isCow) {
		renderCow();
	}
	else if (isAstronaut) {
		renderAstronaut();
	}

	glPopMatrix();
}

void Abductee::renderHuman() {

	glMaterialfv(GL_FRONT, GL_AMBIENT, redAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);

	glPushMatrix();

	// Body
	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	glScalef(0.5, 0.5, 0.3);
	glutSolidCube(1);
	glPopMatrix();

	// Head
	glPushMatrix();
	glTranslatef(0, 0.8, 0);
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	// Arms
	glPushMatrix();
	glTranslatef(0.3, 0.3, 0);
	glScalef(0.1, 0.5, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.3, 0.3, 0);
	glScalef(0.1, 0.5, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.8, 0.9, 0.8);
	// Lower Body
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glScalef(0.5, 0.2, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	// Legs
	glPushMatrix();
	glTranslatef(0.15, -0.35, 0);
	glScalef(0.2, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.15, -0.35, 0);
	glScalef(0.2, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void Abductee::renderCow() {

	glPushMatrix();
	glTranslatef(0, 0.2, 0);

	//Body
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glScalef(1.5, 1, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texh1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	drawTexturedCube(1);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	// Legs
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);

	glTranslatef(-0.6, -0.6, 0.35);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(1.2, 0, 0);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(0, 0, -0.7);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(-1.2, 0, 0);
	glPushMatrix();
	glScalef(0.2, 0.3, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();

	// Tail
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glTranslatef(-0.65, 0.4, 0);
	glRotatef(60, 0, 0, 1);
	glPushMatrix();
	glScalef(1, 0.1, 0.1);
	glRotatef(-90, 0, 1, 0);
	glutSolidCone(1, 1, 10, 1);
	glPopMatrix();
	glPopMatrix();

	// Head
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glTranslatef(1, 0.5, 0);
	glPushMatrix();
	glScalef(0.2, 0.7, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	// Horns
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);

	glTranslatef(0, 0.3, 0.2);

	glPushMatrix();
	glRotatef(-45, 1, 0, 0);
	glPushMatrix();
	glScalef(0.05, 0.05, 0.25);
	glutSolidCone(1, 1, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glTranslatef(0, 0, -0.4);

	glPushMatrix();
	glRotatef(-130, 1, 0, 0);
	glPushMatrix();
	glScalef(0.05, 0.05, 0.25);
	glutSolidCone(1, 1, 10, 1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	// Nose
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, pinkAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pink);
	glTranslatef(0.2, -0.15, 0);
	glScalef(0.1, 0.15, 0.25);
	glRotatef(-90, 0, 1, 0);
	drawCylinderWithLid();
	glPopMatrix();

	// Eyes
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, blueAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glTranslatef(0.125, 0.225, 0.125);
	glPushMatrix();
	glScalef(0.025, 0.05, 0.05);
	glRotatef(-90, 0, 1, 0);
	drawCylinderWithLid();
	glPopMatrix();
	glTranslatef(0, 0, -0.25);
	glPushMatrix();
	glScalef(0.025, 0.05, 0.05);
	glRotatef(-90, 0, 1, 0);
	drawCylinderWithLid();
	glPopMatrix();
	glPopMatrix();

	glPopMatrix(); // end head

	glPopMatrix();
}

void Abductee::drawCylinderWithLid() {
	glPushMatrix();
	gluCylinder(quadric5, 1, 1, 1, 10, 1);
	glRotatef(180, 0, 1, 0);
	gluDisk(quadric5, 0, 1, 10, 1);
	glPopMatrix();
}

void Abductee::renderAstronaut() {
	glPushMatrix();

	// Torso
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glPushMatrix();
	glTranslatef(0, 0.25, 0);
	glScalef(0.6, 0.6, 0.3);
	glutSolidCube(1);
	glPopMatrix();

	// Nasa Logo
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texh2);
	glPushMatrix();
	glTranslatef(0, 0.35, 0.15);
	glRotatef(90, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.1, 0.4, 0.2);
	drawCubeOneSideTextured(1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// Backpack
	glPushMatrix();
	glTranslatef(0, 0.35, -0.25);
	glScalef(0.7, 0.6, 0.25);
	glutSolidCube(1);
	glPopMatrix();
	// Antenna
	glPushMatrix();
	glTranslatef(0.2, 0.5, -0.25);
	drawAntenna(0.5, 0.02, 0.05, red, redAmbient, white, defaultAmbient);
	glPopMatrix();
	// Control Panel (?)
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glPushMatrix();
	glTranslatef(0, 0.4, -0.4);
	glRotatef(-90, 0, 1, 0);
	glScalef(0.1, 0.2, 0.3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texh1);
	drawCubeOneSideTextured(1);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, 0.8, 0);
	drawHelmetAndVisor();
	glPopMatrix();

	// Arms
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glPushMatrix();
	glTranslatef(0.35, 0.3, 0);
	glScalef(0.1, 0.5, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.35, 0.3, 0);
	glScalef(0.1, 0.5, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.9, 0.9, 0.9);
	// Lower Body
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	glScalef(0.5, 0.2, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	// Legs
	glPushMatrix();
	glTranslatef(0.15, -0.35, 0);
	glScalef(0.2, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.15, -0.35, 0);
	glScalef(0.2, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void Abductee::drawHelmetAndVisor() {
	// Helmet - back
	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -0.2);
	glRotatef(90, 1, 0, 0);
	glScalef(0.25, 0.25, 0.25);
	domeHemiSphere->render();
	glPopMatrix();
	glPopMatrix();
	// front
	glPushMatrix();
	glTranslatef(0, 0, 0.05);
	glScalef(0.25, 0.25, 0.25);
	gluDisk(quadric5, 0, 1, 10, 1);
	glPopMatrix();

	// Visor
	glMaterialfv(GL_FRONT, GL_AMBIENT, blueAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, visorOrange);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
}

void Abductee::drawAntenna(float needleLength, float needleRadius, float topBitRadius,
	const GLfloat *topBitColor, const GLfloat *topBitAmbient,
	const GLfloat *needleColor, const GLfloat *needleAmbient) {
	glPushMatrix();
	// Needle
	glMaterialfv(GL_FRONT, GL_AMBIENT, needleAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, needleColor);
	glTranslatef(0, needleLength, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quadric5, needleRadius, needleRadius, needleLength, 10, 1);
	// Top bit
	glTranslatef(0, 0, -topBitRadius);
	//glMaterialfv(GL_FRONT, GL_EMISSION, topBitColor);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_AMBIENT, topBitAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, topBitColor);
	gluSphere(quadric5, topBitRadius, 10, 10);
	glPopMatrix();
}