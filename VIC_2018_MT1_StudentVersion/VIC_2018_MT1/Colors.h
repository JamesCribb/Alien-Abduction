/*** Put the color declarations here to keep main from getting any more cluttered
     than it already is... ***/

#pragma once
#include <stdio.h>
#include <math.h>
#define GLUT_DISABLE_ATEXIT_HACK


// Basic colors
static GLfloat	red[] = { 1.0, 0.0, 0.0, 1 };
static GLfloat  redAmbient[] = { 0.2, 0, 0, 1 };
static GLfloat	blue[] = { 0.0, 0.0, 1.0 };
static GLfloat  blueAmbient[] = { 0, 0, 0.2 };
static GLfloat	green[] = { 0.0, 1.0, 0.0 };
static GLfloat  greenAmbient[] = { 0, 0.2, 0 };
static GLfloat	cyan[] = { 0.0, 1.0, 1.0 };
static GLfloat  cyanAmbient[] = { 0, 0.2, 0.2 };
static GLfloat	magenta[] = { 1.0, 0.0, 1.0 };
static GLfloat  magentaAmbient[] = { 0.2, 0, 0.2 };
static GLfloat	yellow[] = { 1.0, 1.0, 0.0 };
static GLfloat  yellowAmbient[] = { 0.2, 0.2, 0 };
static GLfloat	orange[] = { 1, 0.706, 0 };
static GLfloat  orangeAmbient[] = { 0.2, 0.141, 0 };
static GLfloat	black[] = { 0.0, 0.0, 0.0 };
static GLfloat	white[] = { 1.0, 1.0, 1.0 };
static GLfloat  whiteAmbient[] = { 0.2, 0.2, 0.2 };

// Greyscale stuff
static GLfloat  grey[] = { (float)150 / 255, (float)150 / 255, (float)150 / 255 };
static GLfloat  lightgrey[] = { (float)200 / 255, (float)200 / 255, (float)200 / 255 };
static GLfloat  darkgrey[] = { (float)100 / 255, (float)100 / 255, (float)100 / 255, 0.2 };
static GLfloat  verydarkgrey[] = { (float)50 / 255, (float)50 / 255, (float)50 / 255 };
static GLfloat  greyblue[] = { (float)139 / 255, (float)158 / 255, (float)188 / 255 };

// Big house stuff
static GLfloat  skyBlue[] = { (float)137 / 255, (float)204 / 255, (float)255 / 255, 0.6 };
static GLfloat  skyBlueAmbient[] = { (float)27 / 255, (float)41 / 255, (float)51 / 255, 0.6 };

// Ufo stuff
static GLfloat	ufoBubble[] = { (float)209 / 255, (float)226 / 255 , (float)255 / 255 };
static GLfloat  ufoBubbleAmbient[] = { (float)42 / 255, (float)45 / 255 , (float)51 / 255 };
static GLfloat  brightGreen[] = { (float)73 / 255, (float)255 / 255 , (float)33 / 255 };
//static GLfloat  medGreen[] = { (float)58 / 255, (float)204 / 255 , (float)26 / 255 };
//static GLfloat  dullGreen[] = { (float)47 / 255, (float)163 / 255 , (float)21 / 255 };
static GLfloat  medGreen[] = { (float)22 / 255, (float)77 / 255 , (float)10 / 255 };
static GLfloat  dullGreen[] = { (float)7 / 255, (float)23 / 255 , (float)3 / 255 };

// Tree stuff
static GLfloat  limeGreen[] = { (float)128 / 255, (float)255 / 255 , (float)73 / 255 };
static GLfloat  greyGreen[] = { (float)149 / 255, (float)160 / 255 , (float)130 / 255 };
static GLfloat  brown[] = { (float)104 / 255, (float)82 / 255 , (float)48 / 255 };
static GLfloat  brownAmbient[] = { (float)21 / 255, (float)16 / 255 , (float)10 / 255 };
static GLfloat  lightpink[] = { (float)255 / 255, (float)211 / 255 , (float)249 / 255 };
static GLfloat  pink[] = { (float)239 / 255, (float)162 / 255 , (float)229 / 255 };
static GLfloat  pinkAmbient[] = { (float)48 / 255, (float)32 / 255 , (float)46 / 255 };

// Flag stuff
static GLfloat  oldGloryBlue[] = { 0, (float)40 / 255, (float)104 / 255 };
static GLfloat  oldGloryRed[] = { (float)191 / 255, (float)10 / 255, (float)48 / 255 };
static GLfloat  oldGloryBlueAmbient[] = { 0, ((float)40 * 0.5) / 255, ((float)104 * 0.5) / 255 };
static GLfloat  oldGloryRedAmbient[] = { ((float)191 * 0.5) / 255, ((float)10 * 0.5) / 255,
	((float)48 * 0.5) / 255 };
static GLfloat  whiteFlagAmbient[] = { 0.5, 0.5, 0.5 };
static GLfloat  redFlagAmbient[] = { 0.5, 0, 0 };
static GLfloat  yellowFlagAmbient[] = { 0.5, 0.5, 0 };

static GLfloat  finialBall[] = { 1, (float)199 / 255, 0 };
static GLfloat  finialBallAmbient[] = { 0.2, (float)40 / 255, 0 };

static GLfloat  finialBallSpec[] = { 0.992157, 0.941176, 0.807843};
static GLint    finialBallShin = 3;

// Astronaut stuff
static GLfloat  visorOrange[] = { 0.91, 0.812, 0.69 };

// Default materials values
static GLfloat  defaultAmbient[] = { 0.2, 0.2, 0.2, 1 };
static GLfloat  defaultDiffuse[] = { 0.8, 0.8, 0.8, 1 };
static GLfloat  defaultSpecular[] = { 0, 0, 0, 1 };
static GLfloat  defaultEmission[] = { 0, 0, 0, 1 };
static GLint    defaultShininess = 0;

// Terrain stuff
static GLfloat  land[] = { (float)255 / 255, (float)238 / 255 , (float)193 / 255 };
static GLfloat  landAmbient[] = { (float)51 / 255, (float)47.6 / 255 , (float)38.6 / 255 };
static GLfloat  seaBlue[] = { 0, 0.412, 0.58 };
static GLfloat  seaBlueAmbient[] = { 0, 0.082, 0.016 };
static GLfloat  grass[] = { (float)77 / 255, (float)189 / 255 , (float)51 / 255 };
static GLfloat  grassAmbient[] = { (float)77 / 255 * 0.2, (float)189 / 255 * 0.2, (float)193 / 51 * 0.2 };

// Soil
//static GLfloat  stone[] = { (float)145 / 255, (float)142 / 255 , (float)133 / 255 };
//static GLfloat  stoneAmbient[] = { (float)145 / 255 * 0.2, (float)142 / 255 * 0.2, (float)133 / 255 * 0.2 };

// Grey
//static GLfloat  stone[] = { (float)172 / 255, (float)124 / 255 , (float)66 / 255 };
//static GLfloat  stoneAmbient[] = { (float)172 / 255 * 0.2, (float)124 / 255 * 0.2, (float)66 / 255 * 0.2 };

// Chalk
//static GLfloat  stone[] = { (float)243 / 255, (float)242 / 255 , (float)242 / 255 };
//static GLfloat  stoneAmbient[] = { (float)243 / 255 * 0.2, (float)242 / 255 * 0.2, (float)242 / 255 * 0.2 };

// Grey brown
static GLfloat  stone[] = { (float)132 / 255, (float)116 / 255 , (float)96 / 255 };
static GLfloat  stoneAmbient[] = { (float)132 / 255 * 0.2, (float)116 / 255 * 0.2, (float)96 / 255 * 0.2 };

// Sky colours
//static GLfloat  skyDawn[] = { 1, 0.965, 0.698 };
//static GLfloat  skyDay[] = { 0.529, 0.808, 0.98 };
//static GLfloat  skyDusk[] = { 1, 0.753, 0.627 };
//static GLfloat  skyNight[] = { 0.001, 0.001, 0.118 };
// The actual colour of the sky will be one of these, or a linear interpolation between two

// Alternative sky colours
static GLfloat  skyYellow[] = { (float)247 / 255, (float)255 / 255 , (float)93 / 255 };
static GLfloat  skyGreen[] = { (float)234 / 255, (float)237 / 255 , (float)189 / 255 };
static GLfloat  skyDay[] = { (float)166 / 255, (float)215 / 255 , (float)252 / 255 };
static GLfloat  skyOrange[] = { (float)254 / 255, (float)168 / 255 , (float)90 / 255 };
static GLfloat  skyPink[] = { (float)241 / 255, (float)109 / 255 , (float)125 / 255 };
static GLfloat  skyPurple[] = { (float)57 / 255, (float)27 / 255 , (float)121 / 255 };
static GLfloat  skyNight[] = { (float)4 / 255, (float)12 / 255 , (float)61 / 255 };

// Starts at day
static GLfloat  skyColor[] = { (float)166 / 255, (float)215 / 255 , (float)252 / 255 };

static void interpolateSkyColor(const GLfloat *startColor, const GLfloat *endColor, 
	GLfloat startDegrees, GLfloat endDegrees, GLfloat degrees) {
	GLfloat red = startColor[0] + (float)(((startDegrees - degrees) * ((endColor[0] - startColor[0]) /
		(startDegrees - endDegrees))));
	GLfloat green = startColor[1] + (float)((startDegrees - degrees) * (((endColor[1] - startColor[1]) /
		(startDegrees - endDegrees))));
	GLfloat blue = startColor[2] + (float)(((startDegrees - degrees) * ((endColor[2] - startColor[2]) /
		(startDegrees - endDegrees))));
	skyColor[0] = red;
	skyColor[1] = green;
	skyColor[2] = blue;
}

// Light colours for the day/night cycle
static GLfloat  sunLightAmbient[] = { 0.2, 0.2, 0.18 };
static GLfloat  sunLightDiffuse[] = { 1, 1, 0.88 };
// Start with full brightness
static GLfloat  sunLight[] = { 1, 1, 0.88 };

static GLfloat  moonLightAmbient[] = { 0, 0.2, 0 };
static GLfloat  moonLightDiffuse[] = { (float)93 / 255, (float)98 / 255 , (float)108 / 255 }; // set this to something nicer once it's working
//static GLfloat  moonLight[] = { (float)186 / 255, (float)195 / 255 , (float)216 / 255 };
static GLfloat  moonLight[] = { (float)93 / 255, (float)98 / 255 , (float)108 / 255 };

static void interpolateColor(const GLfloat *startColor, const GLfloat *endColor,
	GLfloat startDegrees, GLfloat endDegrees, GLfloat degrees, GLfloat *colorToChange) {
	GLfloat red = startColor[0] + (float)(((startDegrees - degrees) * ((endColor[0] - startColor[0]) /
		(startDegrees - endDegrees))));
	GLfloat green = startColor[1] + (float)((startDegrees - degrees) * (((endColor[1] - startColor[1]) /
		(startDegrees - endDegrees))));
	GLfloat blue = startColor[2] + (float)(((startDegrees - degrees) * ((endColor[2] - startColor[2]) /
		(startDegrees - endDegrees))));
	colorToChange[0] = red;
	colorToChange[1] = green;
	colorToChange[2] = blue;
}
