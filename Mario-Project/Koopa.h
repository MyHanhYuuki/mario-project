#pragma once

#include "GameObject.h"
#include "Goomba.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_SHELL_MOVING_SPEED 0.15f

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 27
#define KOOPA_BBOX_HEIGHT_SHELL 16
#define KOOPA_BBOX_HEIGHT_DIE 7

#define KOOPA_DIE_TIMEOUT 500
#define KOOPA_INSIDE_SHELL_TIMEOUT 5000

#define KOOPA_STATE_WALKING 1
#define KOOPA_STATE_SHELL 2
#define KOOPA_STATE_SHELL_MOVING 3
#define KOOPA_STATE_SHELL_TRANFORM 4
#define KOOPA_STATE_DIE 200

#define ID_ANI_KOOPA_WALKING_LEFT 11003
#define ID_ANI_KOOPA_WALKING_RIGHT 11004
#define ID_ANI_KOOPA_SHELL 11005
#define ID_ANI_KOOPA_SHELL_MOVING 11006
#define ID_ANI_KOOPA_SHELL_TRANFORM 11007
#define ID_ANI_KOOPA_DIE 5001

class CKoopa : public CGameObject
{
private:
	bool isBeingHold;
	bool isOnFlatform;
	DWORD64 onShellStart;

	int GetAnimationID();

	void OnCollisionWithInvisibleFlatform(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
public:
    CKoopa(float x, float y) : CGameObject(x, y) {
		this->ax = 0;
		this->ay = KOOPA_GRAVITY;
		this->isOnFlatform = false;
		this->isBeingHold = false;
		
		// TODO: set dynamicly base on koppa type (from name)
		this->height = KOOPA_BBOX_HEIGHT;
		this->width  = KOOPA_BBOX_WIDTH;

		this->onShellStart = 0;

		SetState(KOOPA_STATE_WALKING);
	}

    virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return !isBeingHold; };
	virtual int IsBlocking() { return !isBeingHold; }

	virtual void SetState(int state);
	virtual void SetBeingHold(bool isBeingHold) { this->isBeingHold = isBeingHold; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnDamaged(LPCOLLISIONEVENT event, LPGAMEOBJECT sourceObject);
};
