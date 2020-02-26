#pragma once
#include <vector>
#include <cmath>
#include <gl/freeglut.h>
#include "Ring.h"
#include <iostream>
using namespace std;
class Player
{
	//controls
	/*
	<--/--> z axis rotation
	z/X rotate on y axis
	v/^ up and down
	A/D left and right
	W/S forward and back
	*/
public:
	vector<GLfloat> pos, front, up, right, rot;//position and rotation x,y,z
	GLfloat speed;
	Player(vector<GLfloat> _pos = {0,0,-300});
	void PlayerView();
	GLfloat Disto(vector<GLfloat> _point);
	vector<GLfloat> GetSpherePoint(GLfloat _xAngle, GLfloat _yangle);
	void Update(vector<ring> &rings, int &_currentRing, float & _col);
	void AddVec(vector<GLfloat> _other);
	void SubVec(vector<GLfloat> _other);
};

