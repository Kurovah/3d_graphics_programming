//Windows includes - DO NOT EDIT AS LOTS OF INTERDEPENDENCE
//coursework by - daniel eji w1629904
#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

//  Created by Philip Trwoga on 09/11/2012. Modified on 20/1/16 to include the many VS2015 changes
//  Copyright (c) 2012 __Philip Trwoga__. All rights reserved.
// DO NOT DISTRIBUTE WITHOUT THE AUTHOR'S PERMISSION

#include "3DExample1/Player.h"
#include "3DExample1/ring.h"
#include "3DExample1/Planet.h"
#include "3DExample1/Skybox.h"


//note that this needs gltools.h and gltools.cpp to be included in the shared/library directory

//can define any number of textures here - we just have 2 images
//these are just integers so you can use them as such
#define IMAGE1      0
#define IMAGE2      1
#define IMAGE3      2
#define IMAGE4		3
#define TEXTURE_COUNT 4
GLuint  textures[TEXTURE_COUNT];

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE
//note: for some reason the program can't find textures in the debug folder, or anywhere else for that matter. so the screen will appear black when you turn on the overlay, however this seems inconsistent as the maagma and sky texture got added just fine
const char *textureFiles[TEXTURE_COUNT] = {"../res/floor.tga", "../res/stone.tga","../res/magma.tga","../res/HUD.tga"};


#pragma region lighting stuff
//for lighting if you want to experiment with these
GLfloat mKa[4] = {0.11f,0.06f,0.11f,1.0f}; //ambient
GLfloat mKd[4] = {0.43f,0.47f,0.54f,1.0f}; //diffuse
GLfloat mKs[4] = {1.0f,1.0f,1.0f,1.0f}; //specular
GLfloat mKe[4] = {0.5f,0.5f,0.0f,1.0f}; //emission


// Useful lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	 lightPos[] = { 100.0f, 100.0f, 50.0f, 1.0f };
#pragma endregion


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;

//initialize objects

Player Plyr;
vector<ring> rings;
Planet Plnt({ 100 ,100 , 300});
Skybox skyB;
char pos[7][12], col[2][100];

bool numOn, hudOn , winner, worldInit = false;
int currentRing;
GLfloat crysColl, twe = 0;

#pragma region input stuff
bool inputs[10] = { false,false,false,false,false,false,false,false,false,false };
#define W 0
#define S 1
#define A 2
#define D 3
#define Z 4
#define X 5
#define UP 6
#define DOWN 7
#define LEFT 8
#define RIGHT 9
#pragma endregion

void createWorld() {
	Plyr = Player({ 0,0,-300 });
	rings = {
		ring({0,10,300},{0,0,0},true),
		ring({0,300,200},{0,0,0},false),
		ring({100,60,500},{0,0,0},false),
		ring({100,200,500},{0,90,0},false),
		ring({100,-100,200},{0,0,0},false),
		ring({500,-100,600},{0,90,0},false),
		ring({100,500,500},{0,45,0},false)
	};
	Plnt = Planet({ 200 ,100 , 300 });
	numOn = false; hudOn = false; winner = false;
	currentRing = 0; crysColl = 0;

	system("cls");
	cout << "W/S: forward and back" << endl;
	cout << "A/D: left and right" << endl;
	cout << "Up/Down: Up and down" << endl;
	cout << "Z/X: Yaw" << endl;
	cout << "Left/Right: roll" << endl;
	cout << "Q: Turn on numbers HUD" << endl;
	cout << "E: Toggle overlay" << endl;
	cout << "R: Reset World[when game is done]" << endl;
}
//end of intialisation 

//------------------------------------//
void drawTexturedQuad(int image)
{	
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(1, 0, 1);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(0.0,0.0);
	glVertex3f(0, 0, 0);

    glTexCoord2f(1.0,0.0);
    glVertex3f(800, 0, 0);

    glTexCoord2f(1.0,1.0);
    glVertex3f(800, 600, 0);

    glTexCoord2f(0.0,1.0);
    glVertex3f(0, 600, 0);

    glEnd();
    glDisable( GL_TEXTURE_2D );
}
void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save the previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, 800, 0, 600);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -600, 0);
	glMatrixMode(GL_MODELVIEW);
}
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void displayText(GLfloat x, GLfloat y, GLint r, GLint g, GLint b, const char* string) {
	GLint sLen = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (GLint i = 0; i < sLen; i++)
	{
		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);

	}
}
void displayHUD() {
	glColor3f(1.0, 1.0, 1.0);
	
	sprintf_s(pos[0], "PX:%3.0f", Plyr.pos[0]);
	sprintf_s(pos[1], "PY:%3.0f", Plyr.pos[1]);
	sprintf_s(pos[2], "PZ:%3.0f", Plyr.pos[2]);
	sprintf_s(pos[3], "RX:%3.0f", Plyr.rot[0]);
	sprintf_s(pos[4], "RY:%3.0f", Plyr.rot[1]);
	sprintf_s(pos[5], "RZ:%3.0f", Plyr.rot[2]);
	sprintf_s(col[0], "CRYSTALS COLLECTED:%3.0f", crysColl);
	sprintf_s(col[1], "CRYSTALS LEFT:%3.0f", rings.size() - crysColl);
	glLoadIdentity();
	if (numOn) {
		
		displayText(30, 30, 1, 0, 0, pos[0]);
		displayText(90, 30, 0, 1, 0, pos[1]);
		displayText(150, 30, 0, 0, 1, pos[2]);
		displayText(30, 60, 1, 0, 0, pos[3]);
		displayText(90, 60, 0, 1, 0, pos[4]);
		displayText(150, 60, 0, 0, 1, pos[5]);
		displayText(30, 90, 1, 0, 1, col[0]);
		displayText(30, 120, 1, 0, 1, col[1]);
		glColor3f(0,0.5f,1);

		//back board
		glBegin(GL_LINE_LOOP);
		glVertex2f(20, 10);
		glVertex2f(220, 10);
		glVertex2f(220, 130);
		glVertex2f(20, 130);
		glEnd();

		//middle Line
		glBegin(GL_LINES);
		glVertex2f(20, 70);
		glVertex2f(220, 70);
		glEnd();

#pragma region overlay
		//outer border
		glBegin(GL_LINE_STRIP);
		glVertex2f(220, 10);
		glVertex2f(770, 10);
		glVertex2f(780, 20);
		glVertex2f(780, 580);
		glVertex2f(30, 580);
		glVertex2f(20, 570);
		glVertex2f(20, 130);
		glEnd();

		//center diamond
		glBegin(GL_LINE_LOOP);
		glVertex2f(400, 300 - twe*20);
		glVertex2f(400 + twe * 20, 300);
		glVertex2f(400, 300 + twe * 20);
		glVertex2f(400 - twe * 20, 300);
		glEnd();
		//center reticule
		glBegin(GL_LINE_LOOP);
		glVertex2f(400, 300 - twe * 10);
		glVertex2f(400 + twe * 10, 300);
		glVertex2f(400, 300 + twe * 10);
		glVertex2f(400 - twe * 10, 300);
		glEnd();

		//bottom lines
		glBegin(GL_LINES);
		glVertex2f(400 - twe*30, 340);
		glVertex2f(400 + twe * 30, 340);

		glVertex2f(400 - twe * 40, 360);
		glVertex2f(400 + twe * 40, 360);

		glVertex2f(400 - twe * 60, 380);
		glVertex2f(400 + twe * 60, 380);
		glEnd();

		//side bars
		//big lines
		glBegin(GL_LINES);
		glVertex2f( twe * 310, 10);
		glVertex2f( twe * 310, 580);

		glVertex2f(800 - twe * 310, 10);
		glVertex2f(800 - twe * 310, 580);
		//little lines
		glVertex2f(800 - twe * 310, 260);
		glVertex2f(800 - twe * 305, 260);
		glVertex2f(800 - twe * 310, 300);
		glVertex2f(800 - twe * 305, 300);
		glVertex2f(800 - twe * 310, 340);
		glVertex2f(800 - twe * 305, 340);

		glVertex2f(twe * 310, 260);
		glVertex2f(twe * 305, 260);
		glVertex2f(twe * 310, 300);
		glVertex2f(twe * 305, 300);
		glVertex2f(twe * 310, 340);
		glVertex2f(twe * 305, 340);
		glEnd();

		//side hexes
		glBegin(GL_POLYGON);
		glVertex2f(twe * 20, 240);
		glVertex2f(twe * 40, 260);

		glVertex2f(twe * 40, 340);
		glVertex2f(twe * 20, 360);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex2f(800 - twe * 20, 240);
		glVertex2f(800 - twe * 40, 260);

		glVertex2f(800 - twe * 40, 340);
		glVertex2f(800 - twe * 20, 360);
		glEnd();
#pragma endregion
	}
	if (hudOn) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		drawTexturedQuad(3);
		glDisable(GL_BLEND);
	}
	if (winner) {
		displayText(620,30,1,1,1,"ALL RINGS COLLECTED");
	}
	
	
}
void HandleInput() {
	//FORWARD AND BACK
	if(inputs[W] && !inputs[S]){ Plyr.AddVec(Plyr.right); }
	if(!inputs[W] && inputs[S]){ Plyr.SubVec(Plyr.right); }

	//LEFT AND RIGHT
	if (inputs[A]) { Plyr.AddVec(Plyr.front); }
	if (inputs[D]) { Plyr.SubVec(Plyr.front); }

	//UP AND DOWN
	if (inputs[UP]) { Plyr.AddVec(Plyr.up); }
	if (inputs[DOWN]) { Plyr.SubVec(Plyr.up); }

	//YAW
	if (inputs[Z]) { Plyr.rot[1] -= 0.5f; }
	if (inputs[X]) { Plyr.rot[1] += 0.5f; }

	//ROLL
	if (inputs[LEFT]) { Plyr.rot[2] += 0.5f; }
	if (inputs[RIGHT]) { Plyr.rot[2] -= 0.5f; }
}
void RenderScene(void)
{
 
    // Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	resetPerspectiveProjection();
	//VIEW THE SCENE WITH THE PLAYER
	Plyr.PlayerView();

	//DRAW 3D STUFF
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textures[IMAGE1]);
	for (ring& r : rings) {
		r.Draw();
	}
	glBindTexture(GL_TEXTURE_2D, textures[IMAGE2]);
	skyB.Draw();
	glBindTexture(GL_TEXTURE_2D, textures[IMAGE3]);
	Plnt.Draw();
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//DRAW 2D STUFF
	setOrthographicProjection();
	displayHUD();
	glutSwapBuffers();
	
}
void SetupRC()
{
    //textures

    GLuint texture;
    // allocate a texture name
    glGenTextures( 1, &texture );
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
    // Load textures in a for loop
    glGenTextures(TEXTURE_COUNT, textures);
    //this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
    for(int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        // Bind to next texture object
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);
        
        // Load texture data, set filter and wrap modes
        //note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
        pBytes0 = gltLoadTGA(textureFiles[iLoop],&iWidth, &iHeight,
                             &iComponents, &eFormat);
        
        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);
        
            //set up texture parameters
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //try these too
       // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
        // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        free(pBytes0);
    }
    
	//enable textures
    glEnable(GL_TEXTURE_2D);

    
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
    glFrontFace(GL_CCW);// Counter clock-wise polygons face out
    
	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLightLessBright);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,redLight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
	// Black blue background
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f );
}
void TimerFunc(int value)
{
	//init world
	if (!worldInit) {
		createWorld();
		worldInit = true;
	}
	//FOR WORLD RESETTING
	if (currentRing >= rings.size()) {
		winner = true;
	}
	int i = 0;
	for (ring& r : rings) {
		//setting ring target
		if (i == currentRing) {
			r.target = true;
		}
		else {
			r.target = false;
		}

		//update rings
		r.Update();
		i++;
	};
	HandleInput();
	Plnt.Update();
	Plyr.Update(rings, currentRing, crysColl);
	
	//hud tween
	if (numOn) {
		if (twe < 1) { twe += 0.2f; }
	}
	else {
		if (twe > 0) { twe -= 0.2f; }
	}

    glutPostRedisplay();
    glutTimerFunc(25, TimerFunc, 1);
}
void ChangeSize(int w, int h)
{
    GLfloat fAspect;
    
    // Prevent a divide by zero
    if(h == 0)
        h = 1;
    
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);
    
    // Calculate aspect ratio of the window
    fAspect = (GLfloat)w/(GLfloat)h;
    
    // Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // field of view of 45 degrees, near and far planes 1.0 and 1000
    //that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
    gluPerspective(55.0f, fAspect, 1.0, 2000.0);
    
	// Modelview matrix reset
    glMatrixMode(GL_MODELVIEW);
}
void LetterKeys(unsigned char key, int x, int y) {
	if (key == 'w') {inputs[W] = true;}
	if (key == 'a') {inputs[A] = true;}
	if (key == 's') {inputs[S] = true;}
	if (key == 'd') {inputs[D] = true;}
	if (key == 'z') {inputs[Z] = true;}
	if (key == 'x') {inputs[X] = true;}
	if (key == 'q') { hudOn = !hudOn; }
	if (key == 'e') { numOn = !numOn; }
}
void LetterKeysUp(unsigned char key, int x, int y) {
	if (key == 'w') { inputs[W] = false; }
	if (key == 'a') { inputs[A] = false; }
	if (key == 's') { inputs[S] = false; }
	if (key == 'd') { inputs[D] = false; }
	if (key == 'z') { inputs[Z] = false; }
	if (key == 'x') { inputs[X] = false; }
	if (key == 'r') { if (winner) { createWorld(); } }
}
void ArrowKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP) { inputs[UP] = true; }
	if (key == GLUT_KEY_DOWN) { inputs[DOWN] = true; }
	if (key == GLUT_KEY_LEFT) { inputs[LEFT] = true; }
	if (key == GLUT_KEY_RIGHT) { inputs[RIGHT] = true; }
}
void ArrowKeysUp(int key, int x, int y) {
	if (key == GLUT_KEY_UP) { inputs[UP] = false; }
	if (key == GLUT_KEY_DOWN) { inputs[DOWN] = false; }
	if (key == GLUT_KEY_LEFT) { inputs[LEFT] = false; }
	if (key == GLUT_KEY_RIGHT) { inputs[RIGHT] = false; }
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);// a 4:3 ratio
	glutCreateWindow("Coursework2-Flying ship");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(LetterKeys);
	glutKeyboardUpFunc(LetterKeysUp);
	glutSpecialFunc(ArrowKeys);
	glutSpecialUpFunc(ArrowKeysUp);
    glutTimerFunc(25, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}






