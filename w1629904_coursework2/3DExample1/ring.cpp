#include "ring.h"
ring::ring(vector<GLfloat> _pos, vector<GLfloat> _rot, bool _istarget) {
	//set position and rotation
	pos = _pos;
	rot = _rot;
	target = _istarget;
	done = false;
}

void ring::Update() {
	//rotate ring around zaxis
	rot[2] += 0.3f;
	rotVal++;
}

void ring::Draw() {
	glCullFace(GL_FRONT);
	glColor3f(0, 0.5f, 1);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);//move into position

	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);//yaxis rotation
	glRotatef(rot[2], 0.0f, 0.0f, 1.0f);//zaxis rotation
	glScalef(10, 10, 10);
	DrawToroid(0.25, 1, 8, 8);//draw the tourus

	//color change
	if (target) {
		glColor4f(0, 1, 1, 0.5f);
	}
	else {
		glColor4f(1, 0, 0, 0.5f);
	}
	glPopMatrix();

	if (!done) {
		//draw crystal
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);//move into position
		glRotatef(rotVal, 1.0f, 1.0f, 0.0f);//yaxis rotation
		glScalef(3, 3, 3);
		glutSolidDodecahedron();
		glPopMatrix();
	}
	

}
void ring::DrawToroid( GLfloat tubeRad, GLfloat ringRad, GLint tubeSeg, GLint ringSeg) {
	//NEEDS FIXING
	/*/bind shader to toriod
	Shader shdr("res/BaseShader");
	shdr.Bind();
	/*/
	//bind texture to the toroid
	GLfloat PI = 3.14f;
	GLfloat TAU = 2.0f * PI;

	for (int i = 0; i < tubeSeg; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= ringSeg; j++) {
			for (int k = 0; k <= 1; k++) {
				GLfloat s = (i + k) % tubeSeg + 0.5;
				GLfloat t = j % (ringSeg + 1);

				//for the vertex positions
				GLfloat x = (ringRad + tubeRad * cos(s * TAU / tubeSeg)) * cos(t * TAU / ringSeg);
				GLfloat y = (ringRad + tubeRad * cos(s * TAU / tubeSeg)) * sin(t * TAU / ringSeg);
				GLfloat z = tubeRad * sin(s * TAU / tubeSeg);

				//for texcoords
				GLfloat u = (i + k) / (float)tubeSeg;
				GLfloat v = t / (float)ringSeg;

				glTexCoord2d(u, v);
				glNormal3f(2 * x, 2 * y, 2 * z);
				glVertex3d(2 * x, 2 * y, 2 * z);
			}
		}
		glEnd();
	}
}