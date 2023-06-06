#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_ON_COLLISION 300

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

class CGoomba : public CGameObject
{
protected:
	ULONGLONG die_start;

	// Getters
	virtual int IsCollidable() { return state == GOOMBA_STATE_WALKING; };
	virtual int IsBlocking() { return state == GOOMBA_STATE_WALKING; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual int GetAnimationID()
	{
		int aniId = ID_ANI_GOOMBA_WALKING;

		if (state == GOOMBA_STATE_DIE) {
			aniId = ID_ANI_GOOMBA_DIE;
		}

		return aniId;
	}
	virtual D3DXMATRIX* GetCustomTranformationMatrix()
	{
		D3DXMATRIX result;
		if (state == GOOMBA_STATE_DIE_ON_COLLISION) {
			result = CGame::GetInstance()->GetTranformMatrixForCollisionDeflect(x, y);
		}

		return NULL;
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	// Constructors
	CGoomba(float x, float y) : CGameObject(x, y)
	{
		this->ax = 0;
		this->ay = GOOMBA_GRAVITY;
		die_start = -1;
		SetState(GOOMBA_STATE_WALKING);
	}

	// Setters
	virtual void SetState(int state);

	// Collision related
	virtual void OnDamaged(LPCOLLISIONEVENT e);
};