#pragma once

#include "GameObject.h"

#define ID_ANI_FIREBALL 11002
#define	FIREBALL_WIDTH 8
#define FIREBALL_BBOX_WIDTH 8
#define FIREBALL_BBOX_HEIGHT 8

#define FIREBALL_STATE_BOUNCING 1
#define FIREBALL_STATE_FALL 2
#define FIREBALL_STATE_BOUNCING_END 3

#define FIREBALL_BOUNCING_ACCEL 0.005f
#define FIREBALL_X_ACCEL 0.005f
#define FIREBALL_Y_ACCEL 0.005f
#define FIREBALL_X_MAX_VEL 0.03f
#define FIREBALL_Y_MAX_VEL 0.03f
#define FIREBALL_BOUNCING_AMOUNT 16
#define FIREBALL_VERTICAL_MOVE_OFFSET 0

class CFireBall : public CGameObject
{
private:
    bool isFromPlayer;
public:
    CFireBall(float x, float y, bool isFromPlayer=false) : CGameObject(x, y)
	{
		this->blockDirection = BLOCK_NONE;
        this->isFromPlayer = isFromPlayer;
	}

    void SetState(int state);
	
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	int IsBlocking() { return 0; }
    bool IsFromPlayer() { return this->isFromPlayer; }
};

