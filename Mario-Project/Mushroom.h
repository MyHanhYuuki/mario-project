#pragma once

#include "Game.h"
#include "ComsumableObject.h"
#include "Mario.h"

#define MUSHROOM_RISING_UP_SPEED 0.01f
#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_SPEED_MAX_FALL_DOWN 0.2f
#define MUSHROOM_WALKING_SPEED 0.05f

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define DISTANCE_ADD_GRAVITY MUSHROOM_BBOX_HEIGHT + 1

#define ID_ANI_MUSHROOM 12000

#define MUSHROOM_STATE_IDLE 0
#define MUSHROOM_STATE_RISING 1
#define MUSHROOM_STATE_MOVING 2

class CMushroom: public CComsumableObject
{
private:
	float ax, ay;
	float default_pos_y;
public:
	CMushroom(float x, float y, CGameObject* container) : CComsumableObject(x, y, container)
	{
		ax = 0;
		ay = 0;
		default_pos_y = y;
		SetState(MUSHROOM_STATE_IDLE);
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking() { return 0; }
	virtual void SetState(int state);

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

