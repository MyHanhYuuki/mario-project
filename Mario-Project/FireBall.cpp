#include "FireBall.h"
#include "Utils.h"
#include "PlayScene.h"

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == FIREBALL_STATE_BOUNCING) {
		if (originalY - y >= FIREBALL_BOUNCING_AMOUNT) {
			SetState(FIREBALL_STATE_FALL);
		}
	}

	else if (state == FIREBALL_STATE_FALL) {
		if (y - originalY >= -FIREBALL_VERTICAL_MOVE_OFFSET) {
			SetState(FIREBALL_STATE_BOUNCING_END);
		}
	}

	vy = Clamp(vy, -FIREBALL_Y_MAX_VEL, FIREBALL_Y_MAX_VEL);
	
	x += vx * dt;
	y += vy * dt;
};

void CFireBall::SetState(int state)
{ 
	this->state = state;

	switch (state) {
		case FIREBALL_STATE_BOUNCING:
			ay = -FIREBALL_BOUNCING_ACCEL;
			break;
		case FIREBALL_STATE_FALL:
			ay = FIREBALL_BOUNCING_ACCEL;
			vy = 0;
			break;
	}
}

void CFireBall::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_FIREBALL)->Render(x, y);

	RenderBoundingBox();
}

void CFireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - FIREBALL_BBOX_WIDTH / 2;
	t = y - FIREBALL_BBOX_HEIGHT / 2;
	r = l + FIREBALL_BBOX_WIDTH;
	b = t + FIREBALL_BBOX_HEIGHT;
}