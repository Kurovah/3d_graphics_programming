#include "Skybox.h"
Skybox::Skybox() {

}
void Skybox::Draw() {
//show insides
		glCullFace(GL_FRONT);
		glColor3f(0.1f, 0, 0.3f);
		glPushMatrix();
		glRotatef(90,1, 0, 0);
		GLUquadricObj* sphere = NULL;
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, true);
		gluQuadricNormals(sphere, GLU_SMOOTH);
		gluSphere(sphere, 1000, 10, 10);
		
		glPopMatrix();
}