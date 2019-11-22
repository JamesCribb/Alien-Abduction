#include "Flagpole.h"
#include "Colors.h"

FlagPole::FlagPole(GLuint texh1, GLuint texh2) {
	axes = new Axes();
	usFlag = new USFlag();
	chinaFlag = new ChinaFlag();
	ufpFlag = new TexturedFlag(texh1);
	alienFlag = new TexturedFlag(texh2);
	quadric = gluNewQuadric();
	flagToRender = 0;
	flagToRenderNext = 1;
	startAnimation = false;
}

#define FlagSpeed 0.1
float flagOffset = 0;
bool flagGoingDown = false;
bool flagGoingUp = false;
int currentFlag;

void FlagPole::render() {

	glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuse);

	glRotatef(90, 1, 0, 0);

	// Base
	// Step 1
	glColor3fv(white);
	gluCylinder(quadric, 1, 1, 0.1, 15, 1);
	glColor3fv(lightgrey);
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	gluDisk(quadric, 0, 1, 15, 1);
	glPopMatrix();
	// Step 2
	glColor3fv(white);
	glTranslatef(0, 0, -0.1);
	gluCylinder(quadric, 0.9, 0.9, 0.1, 15, 1);
	glColor3fv(lightgrey);
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	gluDisk(quadric, 0, 0.9, 15, 1);
	glPopMatrix();
	// Step 3
	glColor3fv(white);
	glTranslatef(0, 0, -0.1);
	gluCylinder(quadric, 0.8, 0.8, 0.1, 15, 1);
	glColor3fv(lightgrey);
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	gluDisk(quadric, 0, 0.8, 15, 1);
	glPopMatrix();

	// Pole
	glTranslatef(0, 0, -8);
	//glutSolidCylinder(0.05, 8, 10, 1);
	glColor3fv(white);
	gluCylinder(quadric, 0.05, 0.05, 8, 10, 1);

	// Finial Ball
	glTranslatef(0, 0, -0.15);
	glMaterialfv(GL_FRONT, GL_AMBIENT, finialBallAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, finialBall);
	glMaterialfv(GL_FRONT, GL_SPECULAR, finialBallSpec);
	glMateriali(GL_FRONT, GL_SHININESS, finialBallShin);
	glutSolidSphere(0.15, 10, 10);
	glMaterialfv(GL_FRONT, GL_SPECULAR, defaultSpecular);
	glMateriali(GL_FRONT, GL_SHININESS, defaultShininess);

	// Test Flag
	//glColor3f(1, 1, 1);
	//glTranslatef(1.15, 0, 0.75);
	//glScalef(2, 0.1, 1.1);
	//glutSolidCube(1);

	// Flag animation
	glTranslatef(1.35, 0, 0.85);

	if (startAnimation && (flagGoingDown || flagGoingUp)) {
		startAnimation = false;
	}

	if (startAnimation) {
		startAnimation = false;
		flagGoingDown = true;
	}

	if (flagGoingDown) {
		flagOffset += FlagSpeed;
		if (flagOffset > 5) {
			flagGoingDown = false;
			flagGoingUp = true;
			flagToRender = flagToRenderNext;
			flagToRenderNext = ++flagToRenderNext % 4;
		}
	}
	else if (flagGoingUp) {
		flagOffset -= FlagSpeed;
		if (flagOffset < 0) {
			flagGoingUp = false;
		}
	}
	glTranslatef(0, 0, flagOffset);

	glScalef(0.1, 0.1, 0.1);
	if (flagToRender == 0) {
		usFlag->render();
		glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	}
	else if (flagToRender == 1){
		chinaFlag->render();
		glMaterialfv(GL_FRONT, GL_EMISSION, defaultEmission);
	}
	else if (flagToRender == 2) {
		ufpFlag->render(blueAmbient, blue);
	}
	else {
		alienFlag->render(black, black);
	}

	// Static Flag, for testing
	//glTranslatef(1.35, 0, 0.85);
	//glScalef(0.1, 0.1, 0.1);
	//chinaFlag->render();
}