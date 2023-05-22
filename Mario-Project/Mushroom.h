#pragma once
#include "GameObject.h"

#define	MUSHROOM_WIDTH 16
#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_STATE_BOUNCING 2
#define MUSHROOM_STATE_FALL 3
#define MUSHROOM_STATE_BOUNCING_END 4
#define MUSHROOM_STATE_BOUNCING_MOVE 5

#define MUSHROOM_BOUNCING_ACCEL 0.005f
#define MUSHROOM_Y_MAX_VEL 0.2f
#define MUSHROOM_BOUNCING_AMOUNT 12
#define MUSHROOM_VERTICAL_MOVE_OFFSET 8
#define MUSHROOM_X_VEL 0.04f
#define MUSHROOM_FALLING_Y_VEL 0.5f

class CMushroom : public CGameObject
{
protected:
	float ay;
public:
	CMushroom(float x, float y);
    void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);

    // When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt);

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

