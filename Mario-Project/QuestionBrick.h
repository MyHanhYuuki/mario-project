#pragma once

#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#include "Brick.h"
#include "Mushroom.h"


#define QUESTION_BRICK_BOUNCING_UP_SPEED 0.15f
#define QUESTION_BRICK_GRAVITY 0.0002f

#define ID_ANI_QUESTION_BRICK_QUESTION_MARK_MOVING 10100
#define ID_ANI_QUESTION_BRICK_NO_QUESTION_MARK_MOVING 10150
#define QUESTION_BRICK_WIDTH 16
#define QUESTION_BRICK_BBOX_WIDTH 16
#define QUESTION_BRICK_BBOX_HEIGHT 16

#define QUESTION_BRICK_STATE_IDLE_HAVE_ITEM 0
#define	QUESTION_BRICK_STATE_BOUNCING_UP 1
#define QUESTION_BRICK_STATE_IDLE_NO_ITEM 2

class CQuestionBrick : public CBrick
{
private:
	float ay;
	float default_pos_y;

	bool isContainsItem;
	CComsumableObject* item;
public:
	CQuestionBrick(float x, float y, bool isContainsItem) : CBrick(x, y)
	{
		default_pos_y = y;
		ay = 0;

		this->isContainsItem = isContainsItem;
		item = NULL;

		state = isContainsItem ? QUESTION_BRICK_STATE_IDLE_HAVE_ITEM : QUESTION_BRICK_STATE_IDLE_NO_ITEM;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsCollidable() { return 1; };
	virtual int GetAniId();
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

	void ClearItem() { this->item = NULL; }
};

