#include "Leaf.h"
#include "Coin.h"
#include "Utils.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y) {
    blockDirection = BLOCK_NONE;
	ay = 0;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vy = Clamp(vy, -LEAF_Y_MAX_VEL, LEAF_Y_MAX_VEL);

	if (state == LEAF_STATE_BOUNCING) {
		if (originalY - y >= LEAF_BOUNCING_AMOUNT) {
			SetState(LEAF_STATE_FALL);
		}
	} 
	else {
		if (state == LEAF_STATE_FALL) {
			// x = originalX + LEAF_AMPLITUDE * cos((y - originalY)/10 - LEAF_BOUNCING_AMOUNT);
			vx = -LEAF_X_MAX_VEL * sin((y - originalY)/10 - LEAF_BOUNCING_AMOUNT);
			DebugOut(L"%0.3f, %0.3f, %0.3f\n", vx, x, originalX);
		}
    }

	x += vx * dt;
	y += vy * dt;
};

void CLeaf::SetState(int state)
{ 
	this->state = state;

	switch (state) {
		case LEAF_STATE_BOUNCING:
			ay = -LEAF_BOUNCING_ACCEL;
			break;
		case LEAF_STATE_FALL:
			ay = 0;
			vy = LEAF_FALLING_Y_VEL;
			break;
		case LEAF_STATE_FALL_END:
			vx = 0;
			vy = 0;
			break;
	}
}

void CLeaf::Render()
{
    CSprites::GetInstance()->Get(ID_SPRITE_LEAF)->Draw(x, y);

	RenderBoundingBox();
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - LEAF_BBOX_WIDTH / 2;
	t = y - LEAF_BBOX_HEIGHT / 2;
	r = l + LEAF_BBOX_WIDTH;
	b = t + LEAF_BBOX_HEIGHT;
}