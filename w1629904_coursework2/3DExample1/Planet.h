#pragma once
#include <vector>
#include <string>
#include <gl/freeglut.h>
using namespace std;
class Planet
{
public:
	vector<GLfloat> pos, rot;
	Planet(vector<GLfloat>_pos);
	void Update();
	void Draw();
};

