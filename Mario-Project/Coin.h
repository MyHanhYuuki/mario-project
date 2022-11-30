#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#include "BreakableBrick.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

// Time
#define COIN_2_BBRICK_TIME	6000

class CCoin : public CGameObject {
private:
	bool usedToBeBreakableBrick;
	ULONGLONG lifeTime_start;
public:
	CCoin(float x, float y, bool wasBreakableBrick = false) : CGameObject(x, y)
	{
		this->usedToBeBreakableBrick = wasBreakableBrick;
		this->lifeTime_start = 0;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }

	void StartLiving() { this->lifeTime_start = GetTickCount64(); }
};