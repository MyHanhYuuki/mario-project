#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTION_BRICK 10001
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_STATE_NEW 1
#define QUESTION_BRICK_STATE_BOUNCING 2
#define QUESTION_BRICK_STATE_BOUNCING_END 3
#define QUESTION_BRICK_STATE_EMPTY 4

#define QUESTION_BRICK_BOUNCING_ACCEL 0.01f
#define QUESTION_BRICK_BOUNCING_AMOUNT 12

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
	virtual int IsCollidable() { return state == QUESTION_BRICK_STATE_NEW; };
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void Init();
};