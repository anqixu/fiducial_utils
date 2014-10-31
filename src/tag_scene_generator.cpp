#include <GL/glew.h>
#include <GL/glut.h>

#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <cstdio>

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// GLUT callbacks and functions

void initGlut(int argc, char **argv);
void displayFunc(void);
void idleFunc(void);
void reshapeFunc(int width, int height);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void specialFunc(int key, int x, int y);
//-----------------------------------------------------------------------------

// other [OpenGL] functions
void countFrames(void);
void renderBitmapString(float x, float y, float z, void *font, char *string);

//-----------------------------------------------------------------------------

bool bFullsreen = false;
int nWindowID;

//-----------------------------------------------------------------------------

// parameters for the framecounter
char pixelstring[30];
int cframe = 0;
int _time = 0;
int timebase = 0;


//-----------------------------------------------------------------------------

// camera attributes
float viewerPosition[3]		= { 0.0, 0.0, -50.0 };
float viewerDirection[3]	= { 0.0, 0.0, 0.0 };
float viewerUp[3]			= { 0.0, 1.0, 0.0 };

// rotation values for the navigation
float navigationRotation[3]	= { 0.0, 0.0, 0.0 };

// parameters for the navigation

// position of the mouse when pressed
int mousePressedX = 0, mousePressedY = 0;
float lastXOffset = 0.0, lastYOffset = 0.0, lastZOffset = 0.0;
// mouse button states
int leftMouseButtonActive = 0, middleMouseButtonActive = 0, rightMouseButtonActive = 0;
// modifier state
int shiftActive = 0, altActive = 0, ctrlActive = 0;

//-----------------------------------------------------------------------------

// OpenCV variables

IplImage *image = 0;

GLuint cameraImageTextureID;

//-----------------------------------------------------------------------------

bool bInit = false;

//-----------------------------------------------------------------------------

void displayFunc(void) {

	if(!bInit) {

		// load image
		image = cvLoadImage("/tmp/mexico.bmp");

		if(!image)
			std::cout << "ERROR: could not load image" << std::endl;
		
		// initialze OpenGL texture		
		glEnable(GL_TEXTURE_RECTANGLE_ARB);

		glGenTextures(1, &cameraImageTextureID);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, cameraImageTextureID);

		glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		if(image->nChannels == 3)
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, image->width, image->height, 0, GL_BGR, GL_UNSIGNED_BYTE, image->imageData);
		else if(image->nChannels == 4)
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, image->width, image->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->imageData);
			
		bInit = true;
	}

	if(image) {

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_RECTANGLE_ARB);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(50.0, 1.33, 1.0, 100.0);
		
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
	//	gluLookAt(	viewerPosition[0], viewerPosition[1], viewerPosition[2], 
	//				viewerDirection[0], viewerDirection[1], viewerDirection[2], 
	//				viewerUp[0], viewerUp[1], viewerUp[2]);

		glTranslatef( viewerPosition[0], viewerPosition[1], viewerPosition[2] );
		// add navigation rotation
		glRotatef( navigationRotation[0], 1.0f, 0.0f, 0.0f );
		glRotatef( navigationRotation[1], 0.0f, 1.0f, 0.0f );

		// bind texture
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, cameraImageTextureID);
		
		// draw 4 rectangles
		glBegin(GL_QUADS);
			glTexCoord2i(0,image->height);
			glVertex3f(-15.0,-15.0, 15.0);
			glTexCoord2i(image->width,image->height);
			glVertex3f(15.0,-15.0, 15.0);
			glTexCoord2i(image->width,0);
			glVertex3f(15.0,15.0, 15.0);
			glTexCoord2i(0,0);
			glVertex3f(-15.0,15.0, 15.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2i(0,image->height);
			glVertex3f(15.0,-15.0, -15.0);
			glTexCoord2i(image->width,image->height);
			glVertex3f(15.0,-15.0, 15.0);
			glTexCoord2i(image->width,0);
			glVertex3f(15.0,15.0, 15.0);
			glTexCoord2i(0,0);
			glVertex3f(15.0,15.0, -15.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2i(0,image->height);
			glVertex3f(15.0,-15.0, -15.0);
			glTexCoord2i(image->width,image->height);
			glVertex3f(-15.0,-15.0, -15.0);
			glTexCoord2i(image->width,0);
			glVertex3f(-15.0,15.0, -15.0);
			glTexCoord2i(0,0);
			glVertex3f(15.0,15.0, -15.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2i(0,image->height);
			glVertex3f(-15.0,-15.0, -15.0);
			glTexCoord2i(image->width,image->height);
			glVertex3f(-15.0,-15.0, 15.0);
			glTexCoord2i(image->width,0);
			glVertex3f(-15.0,15.0, 15.0);
			glTexCoord2i(0,0);
			glVertex3f(-15.0,15.0, -15.0);
		glEnd();

	}

	glDisable(GL_TEXTURE_RECTANGLE_ARB);

	countFrames();

	glutSwapBuffers();
}

//-----------------------------------------------------------------------------

void initGlut(int argc, char **argv) {

	// GLUT Window Initialization:
	glutInit (&argc, argv);
	glutInitWindowSize (640, 480);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	nWindowID = glutCreateWindow ("simpleGLUT - CvCamera");

	// Register callbacks:
	glutDisplayFunc		(displayFunc);
	glutReshapeFunc		(reshapeFunc);
	glutKeyboardFunc	(keyboardFunc);
	glutSpecialFunc		(specialFunc);
	glutMouseFunc		(mouseFunc);
	glutMotionFunc		(mouseMotionFunc);
	glutIdleFunc		(idleFunc);
}



//-----------------------------------------------------------------------------

void idleFunc(void) {
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------

void reshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------


// mouse callback
void mouseFunc(int button, int state, int x, int y) {
		
	// get the mouse buttons
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {
			leftMouseButtonActive += 1;
		} else
			leftMouseButtonActive -= 1;
	else if (button == GLUT_MIDDLE_BUTTON)
		if (state == GLUT_DOWN) {
			middleMouseButtonActive += 1;
			lastXOffset = 0.0;
			lastYOffset = 0.0;
		} else
			middleMouseButtonActive -= 1;
	else if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {
			rightMouseButtonActive += 1;
			lastZOffset = 0.0;
		} else
			rightMouseButtonActive -= 1;

	mousePressedX = x;
	mousePressedY = y;
}

//-----------------------------------------------------------------------------

void mouseMotionFunc(int x, int y) {
		
	float xOffset = 0.0, yOffset = 0.0, zOffset = 0.0;
	// navigation
	
	// rotatation
	if (leftMouseButtonActive) {

		navigationRotation[0] += ((mousePressedY - y) * 180.0f) / 200.0f;
		navigationRotation[1] += ((mousePressedX - x) * 180.0f) / 200.0f;

		mousePressedY = y;
		mousePressedX = x;

	}
	// panning
	else if (middleMouseButtonActive) {

		xOffset = (mousePressedX + x);
		if (!lastXOffset == 0.0) {
			viewerPosition[0]	-= (xOffset - lastXOffset) / 8.0;
			viewerDirection[0]	-= (xOffset - lastXOffset) / 8.0;
		}
		lastXOffset = xOffset;

		yOffset = (mousePressedY + y);
		if (!lastYOffset == 0.0) {
			viewerPosition[1]	+= (yOffset - lastYOffset) / 8.0;
			viewerDirection[1]	+= (yOffset - lastYOffset) / 8.0;	
		}	
		lastYOffset = yOffset;

	}
	// depth movement
	else if (rightMouseButtonActive) {
		zOffset = (mousePressedX + x);
		if (!lastZOffset == 0.0) {
			viewerPosition[2] -= (zOffset - lastZOffset) / 5.0;
			viewerDirection[2] -= (zOffset - lastZOffset) / 5.0;
		}
		lastZOffset = zOffset;
	}
}

//-----------------------------------------------------------------------------

void keyboardFunc(unsigned char key, int x, int y) {
    
	switch(key) {
		
		// -----------------------------------------

#ifdef WIN32
		// exit on escape
		case '\033':

			if(bInit) {
				if(image)
					cvReleaseImage(&image);
				glDeleteTextures(1, &cameraImageTextureID);
			}
			exit(0);
			break;
#endif

		// -----------------------------------------
			
		// switch to fullscreen
		case 'f':

			bFullsreen = !bFullsreen;
			if(bFullsreen) 
				glutFullScreen();
			else {
				glutSetWindow(nWindowID);
				glutPositionWindow(100, 100);
				glutReshapeWindow(640, 480);
			}
			break;

		// -----------------------------------------
	}
}

//-----------------------------------------------------------------------------

void specialFunc(int key, int x, int y) {
	//printf("key pressed: %d\n", key);
}

//-----------------------------------------------------------------------------

void countFrames(void)  {

	_time=glutGet(GLUT_ELAPSED_TIME);
	cframe++;
	if (_time - timebase > 50) {
		sprintf(pixelstring, "fps: %4.2f", cframe*1000.0/(_time-timebase));		
		timebase = _time;
		cframe = 0;
	// Draw status text and uni-logo:
	} 
	glDisable(GL_LIGHTING);
	glColor4f(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 200, 0, 200);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// render the string
	renderBitmapString(5,5,0.0,GLUT_BITMAP_HELVETICA_10,pixelstring);
		
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

//-----------------------------------------------------------------------------

void renderBitmapString(float x, float y, float z, void *font, char *string) {
  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

//-----------------------------------------------------------------------------


int main(int argc, char **argv) {
	initGlut(argc, argv);
	glutMainLoop();
  return 0;
}
