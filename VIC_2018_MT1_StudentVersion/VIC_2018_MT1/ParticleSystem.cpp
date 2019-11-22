#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g,
	GLfloat b) {
	psPosition[0] = x;
	psPosition[1] = y;
	initialYPos = y;
	psPosition[2] = z;
	targetColor[0] = r;
	targetColor[1] = g;
	targetColor[2] = b;
	isRunningFireworks = false;
	isRunningRocket = false;
	timer = 0;

	resetToRocket();
}

// Reset each particle in the particle system so that it's ready to render the 
// fireworks routine. The basic idea is that each particle gets a vector corresponding 
// to some point on a sphere, and moves along that vector during the render routine
void ParticleSystem::resetToFireworks() {
	for (int i = 0; i < 100; i++) {
		particles[i].position[0] = 0;
		particles[i].position[1] = 0;
		particles[i].position[2] = 0;

		// Algorithm derived from formulae from the following sites:
		// http://mathworld.wolfram.com/SpherePointPicking.html
		// http://mathworld.wolfram.com/SphericalCoordinates.html

		// https://stackoverflow.com/questions/686353/c-random-float-number-generation
		float u = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float v = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		float theta = 2 * M_PI * u;
		float phi = acos((2 * v) - 1);

		particles[i].vector[0] = cos(theta) * sin(phi);
		particles[i].vector[1] = sin(theta) * sin(phi);
		particles[i].vector[2] = cos(phi);
	}
}

void ParticleSystem::renderFireworks() {

	glDisable(GL_LIGHTING);
	glPointSize(2);

	// Interpolate colour
	color[0] = 1 + (timer*1.2 - 0)*((targetColor[0] - 1) / (1 - 0));
	color[1] = 1 + (timer*1.2 - 0)*((targetColor[1] - 1) / (1 - 0));
	color[2] = 1 + (timer*1.2 - 0)*((targetColor[2] - 1) / (1 - 0));
	glColor3fv(color);

	glPushMatrix();

	glTranslatef(psPosition[0], psPosition[1], psPosition[2]);

	glBegin(GL_POINTS);
	for (int i = 0; i < 100; i++) {
		glVertex3f(
			particles[i].vector[0] * timer * 5,
			particles[i].vector[1] * timer * 5,
			particles[i].vector[2] * timer * 5
		);
	}
	glEnd();

	glPopMatrix();

	glPointSize(1);
	glEnable(GL_LIGHTING);

	timer += 0.01;
	if (timer > 0.5) {
		timer = 0;
		isRunningFireworks = false;
		resetToRocket();
		psPosition[1] = initialYPos;
	}
}

// Reset each particle in the particle system so that it's ready to render the 
// rocket routine. The basic idea is that each particle gets a vector corresponding to 
// some point on a circle below the particle system, and moves along that vector
// during the render routine
void ParticleSystem::resetToRocket() {

	for (int i = 0; i < 100; i++) {

		particles[i].position[0] = 0;
		particles[i].position[1] = 0;
		particles[i].position[2] = 0;

		// x can be any number between 0 and 0.25
		GLfloat x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 4;
		// randomise the sign of x
		if (rand() % 2 == 0) {
			x = -(x);
		}
		particles[i].vector[0] = x;

		// y is just some constant value below the origin
		particles[i].vector[1] = -2;

		// z can be any number between 0 and sin(arccos(x))
		GLfloat z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
			* sin(acos(x));
		// Randomise the sign of z
		if (rand() % 2 == 0) {
			z = -(z);
		}
		particles[i].vector[2] = z;
	}
}

void ParticleSystem::renderRocket() {

	glDisable(GL_LIGHTING);
	glPointSize(2);
	glColor3fv(orange);
	glPushMatrix();
	glTranslatef(psPosition[0], psPosition[1], psPosition[2]);

	// number of particles currently rendering
	int limit = timer * 100;

	glBegin(GL_POINTS);
	for (int i = 0; i < limit; i++) {

		particles[i].position[0] += particles[i].vector[0] * 0.01;
		particles[i].position[1] += particles[i].vector[1] * 0.01;
		particles[i].position[2] += particles[i].vector[2] * 0.01;
	}

	for (int i = 0; i < 100; i++) {
		glVertex3f(
			particles[i].position[0] * 3,
			particles[i].position[1] * 3,
			particles[i].position[2] * 3
		);
	}

	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPointSize(1);

	timer += 0.01;
	if (timer > 0.8) {
		timer = 0;
		isRunningRocket = false;
		isRunningFireworks = true;
		resetToFireworks();
	}
}