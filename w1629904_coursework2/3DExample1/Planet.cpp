#include "Planet.h"
Planet::Planet(vector<GLfloat>_pos) {
	pos = _pos;
	rot = { 0,0,0 };
}

void Planet::Update() {
	rot[1]+= 0.1f;
	rot[2] += 0.1f;
}

void Planet::Draw() {
	glCullFace(GL_BACK);
	glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);//move into position
	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);//yaxis rotation
	glRotatef(rot[2], 0.0f, 0.0f, 1.0f);//zaxis rotation
	//glScalef(100, 100, 100);
	GLUquadricObj* sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricTexture(sphere, true);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, 50, 10, 10);
	glPopMatrix();
}
