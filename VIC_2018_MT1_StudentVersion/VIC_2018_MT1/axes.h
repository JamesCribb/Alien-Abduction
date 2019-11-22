#ifndef __drawAXES__
#define __drawAXES__

#define GLUT_DISABLE_ATEXIT_HACK


//#include "renderable.h"

class Axes //: public renderableParent
{
protected:

void drawLabelledAxesP(GLfloat len, GLfloat TxtSize);

public:
GLfloat length;
GLfloat textSize;

void setlen(GLfloat len);
void setTextSize(GLfloat t);

virtual int render();
void drawAxesP(GLfloat len);
Axes();

};

#endif
