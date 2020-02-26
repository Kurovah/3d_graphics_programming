#pragma once
#include <vector>
#include <string>
#include <gl/freeglut.h>
using namespace std;
class ring
{
public:
	vector<GLfloat> pos, rot;//position and rotation x,y,z
	GLfloat rotVal = 0;
	bool target,done;
	vector<vector<GLfloat>> verts;
	ring(vector<GLfloat> _pos, vector<GLfloat> _rot,bool _istarget);
	void Update();
	void Draw();
	void DrawToroid(GLfloat tubeRad = 0.07f, GLfloat ringRad = 0.15f, GLint tubeSeg = 16, GLint ringSeg = 8);
};

