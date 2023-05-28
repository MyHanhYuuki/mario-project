#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#include "BreakableBrick.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 16
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_BOUNCING 2
#define COIN_STATE_FALL 3
#define COIN_STATE_BOUNCING_END 4

#define COIN_BOUNCING_ACCEL 0.03f
#define COIN_Y_MAX_VEL 0.3f
#define COIN_BOUNCING_AMOUNT 64
#define COIN_VERTICAL_MOVE_OFFSET 8

class CCoin : public CGameObject {
private:
	bool usedToBeBreakableBrick;
	ULONGLONG lifeTime_start;
public:
	CCoin(float x, float y, bool wasBreakableBrick = false) : CGameObject(x, y)
	{
		this->usedToBeBreakableBrick = wasBreakableBrick;
		this->lifeTime_start = 0;
		this->blockDirection = BLOCK_NONE;
	}
	
	void SetState(int state);
	
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void StartLiving() { this->lifeTime_start = GetTickCount64(); }
};