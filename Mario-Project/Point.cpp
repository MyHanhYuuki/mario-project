#include "Point.h"
#include "Sprites.h"
#include "Utils.h"

CPoint::CPoint(float x, float y): CGameObject(x, y) {
	ay = 0;
	point = 100;
};

void CPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += ay * dt;

	if (state == POINT_STATE_BOUNCING) {
		if (originalY - y >= POINT_BOUNCING_AMOUNT) {
			SetState(POINT_STATE_BOUNCING_END);
		}
	}

	vy = Clamp(vy, -POINT_Y_MAX_VEL, POINT_Y_MAX_VEL);
	
	x += vx * dt;
	y += vy * dt;
}

void CPoint::Render()
{
	int spriteID = GetSpriteID();
 	CSprites::GetInstance()->Get(spriteID)->Draw(x, y);
}

void CPoint::SetState(int state)
{ 
	this->state = state;

	switch (state) {
		case POINT_STATE_BOUNCING:
			ay = -POINT_BOUNCING_ACCEL;
			break;
		case POINT_STATE_FALL:
			ay = POINT_BOUNCING_ACCEL;
			vy = 0;
			break;
		case POINT_STATE_BOUNCING_END:
			ay = 0;
			vy = 0;
			y = originalY;
			isDeleted = true;
			break;
	}
}