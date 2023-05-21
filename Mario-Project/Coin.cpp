#include "Coin.h"
#include "Utils.h"

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;

	if (state == COIN_STATE_BOUNCING) {
		if (originalY - y >= COIN_BOUNCING_AMOUNT) {
			SetState(COIN_STATE_FALL);
		}
	}

	else if (state == COIN_STATE_FALL) {
		if (y - originalY >= -COIN_VERTICAL_MOVE_OFFSET) {
			// Emit gain point event
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->FireGainPointEvent(
																				EVENT_GAIN_POINT
																				, originalX
																				, originalY - COIN_VERTICAL_MOVE_OFFSET
																			);

			SetState(COIN_STATE_BOUNCING_END);
		}
	}

	vy = Clamp(vy, -COIN_Y_MAX_VEL, COIN_Y_MAX_VEL);
	
	x += vx * dt;
	y += vy * dt;
};

void CCoin::SetState(int state)
{ 
	this->state = state;

	switch (state) {
		case COIN_STATE_BOUNCING:
			ay = -COIN_BOUNCING_ACCEL;
			break;
		case COIN_STATE_FALL:
			ay = COIN_BOUNCING_ACCEL;
			vy = 0;
			break;
		case COIN_STATE_BOUNCING_END:
			ay = 0;
			vy = 0;
			y = originalY;
			isDeleted = true;
			break;
	}
}

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}