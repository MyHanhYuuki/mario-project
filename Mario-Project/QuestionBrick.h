#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTION_BRICK 10001
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_STATE_NEW 1
#define QUESTION_BRICK_STATE_BOUCING 2
#define QUESTION_BRICK_STATE_EMPTY 3

const int BOUNCE_TIME = 50;
const int BOUNCE_DELTA = 24; // pixels
const float BOUNCE_VEL = (float)BOUNCE_DELTA / (float)BOUNCE_TIME ;

class CQuestionBrick : public CGameObject {
private:
	int bounceState;
	float bounceDelta;
	DWORD startBounceTime;

	float ax;
	float ay;

public:
	CQuestionBrick(float x, float y);
	void virtual Render();
	void virtual Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void virtual GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void virtual SetState(int state);

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return state != QUESTION_BRICK_STATE_EMPTY; };
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void Init();
};