#include "Player.h"

Player::Player(vector<GLfloat> _pos) {
	//set position and rotation
	pos = _pos;
	rot = { 0,90,0 };
	front = { 0,0,1 };
	up = { 0,1,0 };
	right = { 1,0,0 };
	speed = 5;
}
void Player::Update(vector<ring>& _rings, int &_currentring, float& _col) {
#pragma region update position and orientation
	//update the Player up right and front vectors
	GLfloat cf = 3.14f / 180.0f;
	//tepmporary up vector to help with gettin right vector
	vector<GLfloat> tempUp = { 0,1,0 };
	//get  the front vector based on pitch and yaw
	front = {
		cosf(rot[2] * cf) * sinf(rot[1] * cf),
		sinf(rot[2] * cf),
		cosf(rot[2] * cf) * cosf(rot[1] * cf)

	};

	//get cross products for other 2 vectors
	right = {
		front[1] * tempUp[2] - front[2] * tempUp[1],
		front[2] * tempUp[0] - front[0] * tempUp[2],
		front[0] * tempUp[1] - front[1] * tempUp[0]
	};
	up = {
		-(front[1] * right[2] - front[2] * right[1]),
		-(front[2] * right[0] - front[0] * right[2]),
		-(front[0] * right[1] - front[1] * right[0])
	};
#pragma endregion
//colliding with ring center
	for(ring& r : _rings){
		if (Disto(r.pos) < 50 && r.target == true ) {
			_currentring++;
			_col++;
			r.done = true;
		}
	}
}
GLfloat Player::Disto(vector<GLfloat> _point) {
	return sqrtf(powf(_point[0] - pos[0], 2) + powf(_point[1] - pos[1], 2)+powf(_point[2] - pos[2], 2));
}
vector<GLfloat> Player::GetSpherePoint(GLfloat _Roll, GLfloat _Yaw) {
	//roll: zaxis rot yaw: yaxis rot
	GLfloat cf = 3.14f / 180.0f;
	return {
		sinf(_Roll * cf) * sinf(_Yaw * cf),
		sinf(_Roll * cf),
		sinf(_Roll * cf) * cosf(_Yaw * cf)
	};
}
void Player::PlayerView() {
	gluLookAt(pos[0], pos[1], pos[2],//camera location
		pos[0] + right[0], pos[1] + right[1], pos[2] + right[2],//where is the camera looking
		up[0], up[1], up[2]);//up vector
}
void Player::AddVec(vector<GLfloat> _other) {
	GLfloat uf = sqrt(pow(_other[0], 2) + pow(_other[1], 2) + pow(_other[2], 2));//to convert into a 'unit vector'
	pos[0] += _other[0] / uf * speed;
	pos[1] += _other[1] / uf * speed;
	pos[2] += _other[2] / uf * speed;
}
void Player::SubVec(vector<GLfloat> _other) {
	GLfloat uf = sqrt(pow(_other[0], 2) + pow(_other[1], 2) + pow(_other[2], 2));
	pos[0] -= _other[0] / uf * speed;
	pos[1] -= _other[1] / uf * speed;
	pos[2] -= _other[2] / uf * speed;
}
