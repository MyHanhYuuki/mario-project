#pragma once
#include "GameObject.h"

#define	LEAF_WIDTH 16
#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14

#define LEAF_STATE_BOUNCING 2
#define LEAF_STATE_FALL 3
#define LEAF_STATE_FALL_END 4

#define LEAF_BOUNCING_ACCEL 0.01f
#define LEAF_Y_MAX_VEL 0.2f
#define LEAF_BOUNCING_AMOUNT 48
#define LEAF_X_MAX_VEL 0.1f
#define LEAF_FALLING_Y_VEL 0.04f

class CLeaf: public CGameObject
{
private:
	float ay;
public:
	CLeaf(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);
};

