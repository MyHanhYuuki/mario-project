#pragma once
#include "Brick.h"
#include "PlayScene.h"

#define BREAKABLE_BRICK_BBOX_WIDTH 16
#define BREAKABLE_BRICK_BBOX_HEIGHT 16

#define ID_ANI_BREAKABLE_BRICK	10200

class CBreakableBrick: public CBrick
{
private:
	bool isWillBecomeNormalBrick;
public:
	CBreakableBrick(float x, float y, int type, bool will_be_normal_brick = false) : CBrick(x, y)
	{
		isWillBecomeNormalBrick = will_be_normal_brick;
	}
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool IsWillBecomeNormalBrick() { return isWillBecomeNormalBrick; }
};

